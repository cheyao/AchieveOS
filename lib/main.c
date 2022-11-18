#include <kernel/keyboard.h>
#include <kernel/idt.h>
#include <stdio.h>

void main(void) {
    update_cursor(0);
    init_idt();

    // char volatile* str = "orld!";
    puts("Hello world");
    // atapi(BUS_PRIMARY, CONTROL_PRIMARY);
}
