#include <kernel/cd.h>
#include <kernel/idt.h>
#include <kernel/keyboard.h>
#include <stdio.h>

Disk disks[4] = {
        {.port = BUS_PRIMARY, .type = NONE, .drive_select_command = 0xA0},
        {.port = BUS_PRIMARY, .type = NONE, .drive_select_command = 0xB0},
        {.port = BUS_SECONDARY, .type = NONE, .drive_select_command = 0xA0},
        {.port = BUS_SECONDARY, .type = NONE, .drive_select_command = 0xB0},
};

void main(void) {
    update_cursor(0);
    init_idt();

    printf("[ATAPI]: Scanning primary bus\n");
    atapi(&disks[0]);
    printf("[ATAPI]: Scanning primary bus slave\n");
    atapi(&disks[1]);
    printf("[ATAPI]: Scanning secondary bus\n");
    atapi(&disks[2]);
    printf("[ATAPI]: Scanning secondary bus slave\n");
    atapi(&disks[3]);
}
