/*
 * AchieveOS main.c
 * Crested by cheyao on 2023.1.1
 */
#include <kernel/ports.h>

static inline void putchar(const char c) {
    outb(UART, c);
}

static inline void puts(const char *str) {
    for (int i = 0; str[i] != 0; i++) {
        putchar(str[i]);
    }
}

void main(void) {
    puts("Hello world!");
}
