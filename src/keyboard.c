//
// Created by cheyao on 11.11.2022.
//

#include "keyboard.h"
#include "types.h"
#include "ports.h"
#include "stdio.h"

const char sc_ascii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                         '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
                         'U', 'I', 'O', 'P', '[', ']', '\n', '?', 'A', 'S', 'D', 'F', 'G',
                         'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
                         'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

void keyboard_call() {
    outb(0x20, 0x20);

    u8int scancode = inb(0x60);

    // if (scancode == 0xE0 && inb(0x60) == 0x11)
    //     printf("Ctrl + ");
    // printf("Received scancode: %d: %c\n", scancode, sc_ascii[scancode]);
}

void update_cursor(u16int pos) {
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t get_cursor_position(void) {
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t) inb(0x3D5)) << 8;
    return pos;
}
