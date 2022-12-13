C_SOURCES = $(wildcard lib/*.c) $(wildcard libc/*.c)
HEADERS = $(wildcard include/*.h) $(wildcard include/kernel/*.h)
OBJ = ${C_SOURCES:.c=.o} lib/idtr.o asmlib/memset.o asmlib/instrset.o asmlib/cachesize.o asmlib/cputype.o \
	asmlib/unalignedisfaster.o asmlib/memcpy.o

CFLAGS = -mno-red-zone -fno-omit-frame-pointer -mfsgsbase -DDEBUG \
		 -nostdlib -ffreestanding -O2 -std=gnu11 -g -static -Wall -Wextra -Wwrite-strings \
		 -Wno-unused-function -Wno-unused-parameter -Wstrict-prototypes -pedantic -Iinclude

LDFLAGS = -T link.ld -ffreestanding -O2 -nostdlib -lgcc -mfsgsbase -mgeneral-regs-only \
          -nostdlib

CC = x86_64-elf-gcc
HOST_CC = gcc-12
AS = nasm
UTILS = util/portions

.PHONY: all clean format

all: cdrom.iso

cdrom.iso: cdrombootsect.bin cdromC.bin cdcontents/bootsect.bin cdcontents/kernel.bin
	dd if=/dev/zero of=cdcontents/kernel.flp bs=512 count=2880
	dd if=cdrombootsect.bin of=cdcontents/kernel.flp conv=notrunc bs=512 seek=0 count=1
	dd if=cdromC.bin of=cdcontents/kernel.flp conv=notrunc bs=512 seek=1 count=2879
	rm cdcontents/.DS_Store; \
	mkisofs -U -o cdrom.iso -V AchiveOS -b kernel.flp cdcontents # Finally a cdrom
	-rm -rf disk.img cdromC.bin cdrombootsect.bin
	qemu-img create disk.img 128M

cdcontents/kernel.bin: lib/kernel_start.o ${OBJ}
	${CC} -o $@ $^ ${LDFLAGS} -z max-page-size=0x1000 -Wl,--oformat=binary

cdromC.bin: boot/cdrom.c boot/cdromstart.asm
	${AS} boot/cdromstart.asm -o boot/cdromstart.o -f elf32
	i686-elf-gcc ${CFLAGS} -m32 -c $< -o boot/cdrom.o
	i686-elf-gcc -o $@ boot/cdromstart.o boot/cdrom.o -z max-page-size=0x1000 -Wl,--oformat=binary -ffreestanding -O2 -nostdlib -lgcc -mfsgsbase -mgeneral-regs-only -nostdlib -T boot/link.ld

cdcontents/bootsect.bin: boot/bootsect.asm
	-mkdir cdcontents
	${AS} $< -f bin -o $@

cdrombootsect.bin: boot/cdromboot.asm
	${AS} $< -f bin -o $@

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -m64 -c $< -o $@

%.o: %.asm
	${AS} -DUNIX -Worphan-labels $< -f elf64 -o $@

clean:
	-rm -rf lib/kernel_start.o ${OBJ} cdromC.bin cdrombootsect.bin boot/cdrom.o boot/cdromstart.o

format:
	clang-format -Werror --style=file -i --verbose ${C_SOURCES} ${HEADERS} boot/cdrom.c
