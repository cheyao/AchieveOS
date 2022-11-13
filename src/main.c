#include "types.h"
#include "keyboard.h"
#include "stdio.h"
#include "idt.h"
#include "time.h"

void mainC(void) {
    update_cursor(0);
    init_idt();
}

void tickC(void) {
    ticks++;
}