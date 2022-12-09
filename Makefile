C_SOURCES = $(wildcard lib/*.c) $(wildcard libc/*.c)
HEADERS = $(wildcard include/*.h) $(wildcard include/kernel/*.h)
OBJ = ${C_SOURCES:.c=.o lib/idtr.o}

CFLAGS = -mno-red-zone -fno-omit-frame-pointer -mfsgsbase -m64 \
		 -nostdlib -ffreestanding -O2 -std=gnu11 -g -static -Wall -Wextra -Wwrite-strings \
		 -Wno-unused-function -Wno-unused-parameter -Wstrict-prototypes -pedantic -Iinclude
		 # -O2 -std=gnu11 -g -static -Wall -Wextra -Wno-unused-function -Wno-unused-parameter \
		 # -Wstrict-prototypes -Wwrite-strings -fno-omit-frame-pointer -mno-red-zone  -mgeneral-regs-only \
		 # -mgeneral-regs-only -nostdlib -Iinclude -Wwrite-strings \
 	     # -fno-stack-protector -fno-builtin -nodefaultlibs -fdiagnostics-show-option \
		 # -fms-extensions -ffreestanding -mcmodel=large -nostartfiles

LDFLAGS = -T link.ld -ffreestanding -O2 -nostdlib -lgcc -mfsgsbase -mgeneral-regs-only \
          -nostdlib

CC = x86_64-elf-gcc
HOST_CC = gcc-12
AS = nasm
UTILS = util/portions

.PHONY: all clean format

all: cdrom.iso

cdrom.iso: bootsect.bin kernel.bin $(UTILS)
	dd if=/dev/zero of=cdcontents/kernel.flp bs=512 count=2880
	dd if=bootsect.bin of=cdcontents/kernel.flp conv=notrunc bs=512 seek=0 count=1
	dd if=kernel.bin of=cdcontents/kernel.flp conv=notrunc bs=512 seek=1 count=`./util/portions \`gstat -L -c %s kernel.bin\``
	rm cdcontents/.DS_Store; \
	mkisofs -U -o cdrom.iso -V AchiveOS -b kernel.flp cdcontents # Finally a cdrom
	-rm -rf disk.img
	qemu-img create disk.img 20M

kernel.bin: lib/kernel_start.o ${OBJ}
	${CC} -o $@ $^ ${LDFLAGS} -z max-page-size=0x1000 -Wl,--oformat=binary

bootsect.bin: boot/bootsect.asm
	${AS} $< -f bin -o $@

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	${AS} $< -f elf64 -o $@

util/portions: util/portions.c
	${HOST_CC} -O3 util/portions.c -o util/portions

clean:
	-rm -rf lib/kernel_start.o ${OBJ}

format:
	clang-format -Werror --style=file -i --verbose ${C_SOURCES} ${HEADERS}
