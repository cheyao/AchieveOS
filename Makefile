export PATH := /usr/local/Cellar/llvm/15.0.7/bin:$(PATH)

C_SOURCES = $(wildcard src/**/*.c) $(wildcard src/*.c)
OBJ = $(subst src/,build/,$(C_SOURCES:.c=.o))

CC := clang
CFLAGS := -fno-omit-frame-pointer -O2 -nostdlib -ffreestanding -std=c2x -static -Wno-unused-parameter --target=riscv64 \
		  -Wno-unused-function -pedantic -Wall -Wextra -Wwrite-strings -Wstrict-prototypes -march=rv64i -mabi=lp64 -flto \
		  -I include -Wno-unused-function -fno-stack-protector -nodefaultlibs \
		  -fms-extensions
TIDY := -std=c2x -static -Wno-unused-parameter --target=riscv64 \
		-Wno-unused-function -pedantic -Wall -Wextra -Wwrite-strings -Wstrict-prototypes -march=rv64i -mabi=lp64 -flto \
		-I include -Wno-unused-function -fno-stack-protector -nodefaultlibs \
		-fms-extensions --use-color
AS := riscv64-unknown-elf-as
ASFLAGS := -march=rv64i -mabi=lp64
LD := ld.lld
LDFLAGS := -T link.ld -nostdlib

.PHONY: all clean clang-tindy

all: 
	echo $(C_SOURCES)
	@mkdir -p build
	make kernel

kernel: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

build/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) -mcmodel=medany -c $< -o $@

build/%.o: src/%.S
	$(AS) $(ASFLAGS) -c $< -o $@

clang-tidy: $(C_SOURCES)
	/usr/local/Cellar/llvm/15.0.7/bin/clang-tidy $(TIDY) $^

clean:
	-rm -rf kernel $(OBJ)
