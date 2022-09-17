CFLAGS = -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs \
         -Wall -Wextra -Werror -I makeFileDir -fms-extensions -ffreestanding -g -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
C_SOURCES = $(wildcard kernel/*.c)
HEADERS = $(wildcard kernel/*.h)
OBJ = ${C_SOURCES:.c=.o kernel/idt_asm.o}

.PHONY: all debug clean

all: OS.iso

debug:OS.iso kernel/kernel.elf
	qemu-system-i386 -s -drive file=boot.iso,format=raw,media=disk -d cpu &
	i686-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file kernel/kernel.elf"

OS.iso: boot/bootsect.bin kernel/kernel.bin
	dd if=/dev/zero of=pacman.iso bs=512 count=9001
	dd if=boot/bootsect.bin of=pacman.iso conv=notrunc bs=512 seek=0 count=1
	dd if=kernel/kernel.bin of=pacman.iso conv=notrunc bs=512 seek=1 count=9000

boot/bootsect.bin:
	nasm -f bin boot/bootsect.asm -o $@

kernel/kernel.bin: kernel/kernel.o ${OBJ}
	i686-elf-ld -o $@ $^ -T link.ld

%.o: %.c ${HEADERS}
	i686-elf-gcc ${CFLAGS} -ffreestanding -c $< -o $@


%.o: %.asm
	nasm $< -f elf32 -o $@

kernel/kernel.elf: kernel/kernel.o ${OBJ}
	i686-elf-ld -o $@ $^ -T link.ld --oformat elf64-x86-64

clean:
	-rm -rf kernel/*.bin boot/*.bin kernel/*.o
	-rm -rf OS.iso boot.iso
