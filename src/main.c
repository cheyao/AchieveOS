#include "types.h"
#include "keyboard.h"
#include "stdio.h"
#include "idt.h"

static u64int ticks = 0;

void mainC(void) {
    update_cursor(0);
    init_idt();

    // __asm__ __volatile__ ("int $0");

    printf("Hello %s%%world!", "Hello world");
}

void tickC(void) {
    ticks++;
}