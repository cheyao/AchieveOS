C_SOURCES = $(wildcard lib/*.c) $(wildcard libc/*.c)
HEADERS = $(wildcard include/*.h) $(wildcard include/kernel/*.h)
OBJ = ${C_SOURCES:.c=.o lib/idtr.o}

CFLAGS = -O1 -std=gnu11 -g -static -Wall -Wextra -Wno-unused-function -Wno-unused-parameter \
		 -Wstrict-prototypes -Wpointer-arith -Wcast-align -Wwrite-strings -Wshadow \
 	     -fverbose-asm -nostdlib -nostdinc -fno-stack-protector -nostartfiles -Wundef \
		 -nodefaultlibs -fno-builtin -fms-extensions -ffreestanding -mcmodel=large \
		 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -Iinclude -Wfloat-equal

LDFLAGS = -T link.ld

CC = x86_64-elf-gcc
LD = x86_64-elf-ld
AS = nasm
UTILS = util/portions

.PHONY: all clean format

all: cdrom.iso

cdrom.iso: bootsect.bin kernel.bin $(UTILS)
	dd if=/dev/zero of=cdcontents/kernel.iso bs=512 count=2880
	dd if=bootsect.bin of=cdcontents/kernel.iso conv=notrunc bs=512 seek=0 count=1
	dd if=kernel.bin of=cdcontents/kernel.iso conv=notrunc bs=512 seek=1 count=`./util/portions \`gstat -L -c %s kernel.bin\``
	mkisofs -o cdrom.iso -V AchiveOS -b kernel.iso cdcontents # Finally a cdrom

kernel.bin: lib/kernel_start.o ${OBJ}
	${LD} -o $@ $^ ${LDFLAGS} --oformat binary

elf: lib/kernel_start.o ${OBJ}
	${LD} -o kernel.elf $^ ${LDFLAGS} --oformat elf64-x86-64

bootsect.bin: boot/bootsect.asm
	nasm $< -f bin -o $@

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	${AS} $< -f elf64 -o $@

util/portions: util/portions.c
	gcc-12 -O3 util/portions.c -o util/portions

clean:
	-rm -rf ${OBJ} kenel.bin bootsect.bin kernel.iso

format:
	clang-format -Werror --style=file -i --verbose ${C_SOURCES} ${HEADERS}
