#include <kernel/keyboard.h>
#include <kernel/idt.h>
#include <kernel/cd.h>
#include <stdio.h>

void main(void) {
    update_cursor(0);
    init_idt();

    printf("[ATAPI]: Scanning primary bus\n");
    atapi(makedisk(BUS_PRIMARY, 0xA0));
    printf("[ATAPI]: Scanning primary bus slave\n");
    atapi(makedisk(BUS_PRIMARY, 0xB0));
    printf("[ATAPI]: Scanning secondary bus\n");
    atapi(makedisk(BUS_SECONDARY, 0xA0));
    printf("[ATAPI]: Scanning secondary bus slave\n");
    atapi(makedisk(BUS_SECONDARY, 0xB0));
}
