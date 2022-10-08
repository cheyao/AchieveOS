C_SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o}

CFLAGS = -O3 -fverbose-asm -nostdlib -nostdinc -fno-stack-protector -nostartfiles \
		 -nodefaultlibs -fno-builtin -fms-extensions -ffreestanding -g

.PHONY: all clean debug

all: OS.iso

debug: src/kernel.elf OS.iso

OS.iso: src/bootsect.bin src/kernel.bin
	dd if=/dev/zero of=OS.iso bs=512 count=901
	dd if=src/bootsect.bin of=OS.iso conv=notrunc bs=512 seek=0 count=1
	dd if=src/kernel.bin of=OS.iso conv=notrunc bs=512 seek=1 count=900

src/kernel.bin: src/kernel_start.o ${OBJ}
	i686-elf-ld -o $@ $^ -T link.ld --oformat binary -melf_i386

src/kernel.elf: src/kernel_start.o ${OBJ}
	i686-elf-ld -o $@ $^ -T link.ld --oformat elf32-i386 -melf_i386

src/%.o: src/%.c ${HEADERS}
	i686-elf-gcc ${CFLAGS} -c $< -o $@

src/%.o: src/%.asm
	nasm $< -f elf32 -o $@

src/%.bin: src/%.asm
	nasm $< -f bin -o $@

clean:
	-rm -rf src/*.bin src/*.o src/*.elf
