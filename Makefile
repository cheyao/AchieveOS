C_SOURCES = $(wildcard kernel/**/**/*.c) $(wildcard kernel/**/*.c) $(wildcard kernel/*.c)
HEADERS = $(wildcard include/**/*.h) $(wildcard include/*.h)
ASMLIB =  $(wildcard kernel/asm/*.asm)
OBJ = ${C_SOURCES:.c=.o} kernel/lib/idtr.o ${ASMLIB:.asm=.o}

CFLAGS = -fno-omit-frame-pointer -DDEBUG -isystem ./include -O2 \
		 -nostdlib -ffreestanding -std=gnu17 -g -static -Wno-unused-parameter -nostdinc -march=rv64id \
		 -Wno-unused-function -pedantic -Wall -Wextra -Wwrite-strings -Wstrict-prototypes

LDFLAGS = -T link.ld -ffreestanding -O2 -nostdlib -lgcc -mfsgsbase -mgeneral-regs-only \
          -nostdlib

CC = riscv64-unknown-elf-gcc
AS = riscv64-unknown-elf-as
UTILS = util/portions

.PHONY: all clean format gdb

all: disk.img

gdb: disk.img
	${x86_CC} -o $@ $^ ${LDFLAGS} -z max-page-size=0x1000
	qemu-system-x86_64 -drive file=disk.img,media=disk,format=raw -cdrom cdrom.iso -m 512 -cpu host -accel hvf -debugcon stdio -boot order=cd -s -S &
	x86_64-elf-gdb cdcontents/kernel.bin -ex "target remote localhost:1234"

disk.img: cdcontents/bootsect.bin cdcontents/second.bin cdcontents/kernel.bin
	qemu-img create disk.img 64M
	dd if=cdcontents/bootsect.bin of=disk.img conv=notrunc bs=512 seek=0 count=1
	dd if=cdcontents/second.bin of=disk.img conv=notrunc bs=512 seek=0x20 count=$$((`gstat -L -c %s cdcontents/second.bin` / 512 + 1))
	dd if=cdcontents/kernel.bin of=disk.img conv=notrunc bs=512 seek=0x70 count=$$((`gstat -L -c %s cdcontents/kernel.bin` / 512 + 1))
	-rm -rf cdromC.bin cdrombootsect.bin

cdcontents/kernel.bin: kernel/kernel_start.o $(OBJ)
	${x86_CC} -o $@ $^ ${LDFLAGS} -z max-page-size=0x1000

cdcontents/second.bin: boot/start.asm boot/main.c
	${AS} boot/start.asm -o boot/start.o -f elf64
	${x86_CC} ${CFLAGS} -m64 -c boot/main.c -o boot/main.o
	${x86_CC} -o $@ boot/start.o boot/main.o -T boot/link.ld -Wl,--oformat=binary -z max-page-size=0x1000 -ffreestanding -O2 -nostdlib -lgcc -mfsgsbase -mgeneral-regs-only -nostdlib

cdcontents/bootsect.bin: boot/bootsect.asm
	-mkdir cdcontents
	${AS} $< -f bin -o $@

%.o: %.c $(HEADERS)
	${CC} ${CFLAGS} -mcmodel=medany -c $< -o $@

%.o: %.asm
	${AS} -c $< -o $@

clean:
	-rm -rf ${OBJ}

format:
	clang-format -Werror --style=file -i --verbose ${C_SOURCES} ${HEADERS} cdrom/main.c
