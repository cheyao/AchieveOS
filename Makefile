C_SOURCES = $(wildcard lib/*.c) $(wildcard libc/*.c)
HEADERS = $(wildcard include/*.h) $(wildcard include/kernel/*.h)
OBJ = ${C_SOURCES:.c=.o} lib/idtr.o asmlib/memset.o asmlib/instrset.o asmlib/cachesize.o asmlib/cputype.o \
	asmlib/unalignedisfaster.o asmlib/memcpy.o

CFLAGS = -mno-red-zone -fno-omit-frame-pointer -mfsgsbase -DDEBUG -Iinclude -O2 \
		 -nostdlib -ffreestanding -std=gnu11 -g -static -Wno-unused-parameter -g \
		 -Wno-unused-function -pedantic -Wall -Wextra -Wwrite-strings -Wstrict-prototypes

LDFLAGS = -T link.ld -ffreestanding -O2 -nostdlib -lgcc -mfsgsbase -mgeneral-regs-only \
          -nostdlib

x86_CC ?= x86_64-elf-gcc
i686_CC ?= i686-elf-gcc
AS = nasm
UTILS = util/portions

.PHONY: all clean format gdb

all: cdrom.iso

gdb: cdrom.iso
	${x86_CC} -o $@ $^ ${LDFLAGS} -z max-page-size=0x1000
	qemu-system-x86_64 -drive file=disk.img,media=disk,format=raw -cdrom cdrom.iso -m 512 -cpu host -accel hvf -debugcon stdio -boot order=cd -s -S &
	x86_64-elf-gdb cdcontents/kernel.bin -ex "target remote localhost:1234"

cdrom.iso: cdrom/bootsect.bin cdrom/kernel.bin cdcontents/bootsect.bin cdcontents/kernel.bin cdcontents/second.bin
	dd if=/dev/zero of=cdcontents/kernel.flp bs=512 count=2880
	dd if=cdrom/bootsect.bin of=cdcontents/kernel.flp conv=notrunc bs=512 seek=0 count=1
	dd if=cdrom/kernel.bin of=cdcontents/kernel.flp conv=notrunc bs=512 seek=1 count=2879
	rm cdcontents/.DS_Store; \
	mkisofs -U -o cdrom.iso -V AchiveOS -b kernel.flp cdcontents # Finally a cdrom
	-rm -rf disk.img cdromC.bin cdrombootsect.bin
	qemu-img create disk.img 128M

cdcontents/kernel.bin: lib/kernel_start.o ${OBJ}
	${x86_CC} -o $@ $^ ${LDFLAGS} -z max-page-size=0x1000

cdcontents/second.bin: boot/start.asm boot/main.c
	${AS} boot/start.asm -o boot/start.o -f elf64
	${x86_CC} ${CFLAGS} -m64 -c boot/main.c -o boot/main.o
	${x86_CC} -o $@ boot/start.o boot/main.o -T boot/link.ld -Wl,--oformat=binary -z max-page-size=0x1000 -ffreestanding -O2 -nostdlib -lgcc -mfsgsbase -mgeneral-regs-only -nostdlib


cdrom/kernel.bin: cdrom/main.c cdrom/start.asm
	${AS} cdrom/start.asm -o cdrom/start.o -f elf32
	${i686_CC} ${CFLAGS} -m32 -c cdrom/main.c -o cdrom/main.o
	${i686_CC} -o $@ cdrom/start.o cdrom/main.o -T boot/link.ld -Wl,--oformat=binary -z max-page-size=0x1000 -ffreestanding -O2 -nostdlib -lgcc -mfsgsbase -mgeneral-regs-only -nostdlib

cdrom/bootsect.bin: cdrom/bootsect.asm
	${AS} $< -f bin -o $@

cdcontents/bootsect.bin: boot/bootsect.asm
	-mkdir cdcontents
	${AS} $< -f bin -o $@

%.o: %.c ${HEADERS}
	${x86_CC} ${CFLAGS} -mcmodel=large -m64 -c $< -o $@

%.o: %.asm
	${AS} -DUNIX -Worphan-labels $< -f elf64 -o $@

clean:
	-rm -rf lib/kernel_start.o ${OBJ} cdromC.bin cdrombootsect.bin cdrom/*.o cdrom/*.bin kernel.elf kernel.bin

format:
	clang-format -Werror --style=file -i --verbose ${C_SOURCES} ${HEADERS} cdrom/main.c
	include-what-you-use cdrom/main.c ${C_SOURCES} ${HEADERS}
