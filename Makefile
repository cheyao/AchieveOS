C_SOURCES = $(wildcard kernel/**/**/*.c) $(wildcard kernel/**/*.c) $(wildcard kernel/*.c)
HEADERS = $(wildcard include/**/*.h) $(wildcard include/*.h)
ASMLIB =  $(wildcard kernel/asm/*.asm)
OBJ = ${C_SOURCES:.c=.o} kernel/lib/idtr.o ${ASMLIB:.asm=.o}

CFLAGS = -fno-omit-frame-pointer -O2 -nostdlib -ffreestanding -std=c17 -static -Wno-unused-parameter \
	     -Wno-unused-function -pedantic -Wall -Wextra -Wwrite-strings -Wstrict-prototypes -march=rv64i -mabi=lp64

LDFLAGS = -T link.ld -ffreestanding -O2 -nostdlib -lgcc -mfsgsbase -mgeneral-regs-only \
          -nostdlib

CC = riscv64-unknown-elf-gcc
AS = riscv64-unknown-elf-as
UTILS = util/portions

.PHONY: all clean format gdb

all: kernel

kernel: kernel/kernel_start.o $(OBJ)
	${x86_CC} -o $@ $^ ${LDFLAGS} -z max-page-size=0x1000

%.o: %.c $(HEADERS)
	mkdir -p build
	${CC} ${CFLAGS} -mcmodel=medany -c $< -o $@

%.o: %.asm
	mkdir -p build
	${AS} -c $< -o $@

clean:
	-rm -rf ${OBJ}

format:
	clang-format -Werror --style=file -i --verbose ${C_SOURCES} ${HEADERS} cdrom/main.c
