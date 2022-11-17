#include "keyboard.h"
#include "idt.h"
#include "time.h"
#include "fs.h"

void mainC(void) {
    update_cursor(0);
    init_idt();

    read(BUS_PRIMARY, CONTROL_PRIMARY);
}

void tickC(void) {
    ticks++;
}