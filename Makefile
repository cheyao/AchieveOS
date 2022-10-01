.PHONY: all clean

all: clean run

run: OS.iso
	qemu-system-x86_64 -vnc 127.0.0.1:0 -hdd pacman.iso

OS.iso: bootsect.bin kernel.bin
	dd if=/dev/zero of=pacman.iso bs=512 count=901
	dd if=bootsect.bin of=pacman.iso conv=notrunc bs=512 seek=0 count=1
	dd if=kernel.bin of=pacman.iso conv=notrunc bs=512 seek=1 count=900


bootsect.bin: 
	nasm -f bin bootsect.asm -o $@

kernel.bin:
	nasm -f bin kernel.asm -o $@

clean:
	-rm -rf *.bin
