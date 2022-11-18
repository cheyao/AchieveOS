C_SOURCES = $(wildcard lib/*.c) $(wildcard libc/*.c)
HEADERS = $(wildcard include/*.h) $(wildcard include/kernel/*.c)
OBJ = ${C_SOURCES:.c=.o lib/idtr.o}

CFLAGS = -O1 -fverbose-asm -nostdlib -nostdinc -fno-stack-protector -nostartfiles \
		 -nodefaultlibs -fno-builtin -fms-extensions -ffreestanding -mcmodel=large \
		 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -Iinclude
LDFLAGS = -T link.ld

CC = x86_64-elf-gcc
LD = x86_64-elf-ld
UTILS = util/portions

.PHONY: all clean

all: OS.iso

OS.iso: bootsect.bin kernel.bin $(UTILS)
	dd if=/dev/zero of=OS.iso bs=512 count=501
	dd if=bootsect.bin of=OS.iso conv=notrunc bs=512 seek=0 count=1
	dd if=kernel.bin of=OS.iso conv=notrunc bs=512 seek=1 count=`./util/portions \`gstat -L -c %s kernel.bin\``

kernel.bin: lib/kernel_start.o ${OBJ}
	${LD} -o $@ $^ ${LDFLAGS}

bootsect.bin: boot/bootsect.asm
	nasm $< -f bin -o $@

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm $< -f elf64 -o $@

util/portions: util/portions.c
	gcc-12 -O3 util/portions.c -o util/portions

clean:
	-rm -rf ${OBJ} kenel.bin bootsect.bin OS.iso
