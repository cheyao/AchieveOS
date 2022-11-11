#include "types.h"
#include "keyboard.h"
#include "stdio.h"

static u64int ticks = 0;

void mainC(void) {
    update_cursor(0);

    printf("Hello world!");
}

void tickC(void) {
    ticks++;
}