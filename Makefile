C_SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)
OBJ = ${C_SOURCES:.c=.o src/idtr.o}

CFLAGS = -O0 -fverbose-asm -nostdlib -nostdinc -fno-stack-protector -nostartfiles \
		 -nodefaultlibs -fno-builtin -fms-extensions -ffreestanding -g -mcmodel=large \
		 -mno-red-zone -mno-mmx -mno-sse -mno-sse2
LDFLAGS = -T link.ld

CC = x86_64-elf-gcc
LD = x86_64-elf-ld

.PHONY: all clean

all: OS.iso

OS.iso: src/bootsect.bin src/kernel.bin
	dd if=/dev/zero of=OS.iso bs=512 count=901
	dd if=src/bootsect.bin of=OS.iso conv=notrunc bs=512 seek=0 count=1
	dd if=src/kernel.bin of=OS.iso conv=notrunc bs=512 seek=1 count=900

src/kernel.bin: src/kernel_start.o ${OBJ}
	${LD} -o $@ $^ ${LDFLAGS}

src/%.o: src/%.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

src/%.o: src/%.asm
	nasm $< -f elf64 -o $@

src/bootsect.bin: src/bootsect.asm
	nasm $< -f bin -o $@

clean:
	-rm -rf src/*.bin src/*.o src/*.elf
