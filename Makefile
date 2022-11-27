C_SOURCES = $(wildcard lib/*.c) $(wildcard libc/*.c)
HEADERS = $(wildcard include/*.h) $(wildcard include/kernel/*.h)
OBJ = ${C_SOURCES:.c=.o lib/idtr.o}

CFLAGS = -O2 -std=gnu11 -g -static -Wall -Wextra -Wno-unused-function -Wno-unused-parameter \
		 -Wstrict-prototypes -Wpointer-arith -Wcast-align -Wwrite-strings -Wshadow \
 	     -fno-stack-protector -Wundef -nostdlib -fno-builtin -nodefaultlibs \
		 -fms-extensions -ffreestanding -mcmodel=large -fverbose-asm -nostartfiles \
		 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -Iinclude -Wfloat-equal
# -nostdinc
LDFLAGS = -T link.ld

CC = x86_64-elf-gcc
HOST_CC = gcc-12
LD = x86_64-elf-ld
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
	-rm -rf disk.img bootsect.bin kernel.bin
	qemu-img create disk.img 20M

kernel.bin: lib/kernel_start.o ${OBJ}
	${LD} -o $@ $^ ${LDFLAGS} --oformat binary

elf: lib/kernel_start.o ${OBJ}
	${LD} -o kernel.elf $^ ${LDFLAGS} --oformat elf64-x86-64

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
