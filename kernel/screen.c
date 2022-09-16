//
// Created by cheyao on 16.09.2022.
//

#include "screen.h"
#include "ports.h"

void init_pallet() {
    outb(PALETTE_MASK, 0xFF);
    outb(PALETTE_WRITE, 0);

    outb(PALETTE_DATA, 0x5F >> 2);
    outb(PALETTE_DATA, 0x96 >> 2);
    outb(PALETTE_DATA, 0xF9 >> 2);

    outb(PALETTE_DATA, 0x00 >> 2);
    outb(PALETTE_DATA, 0x00 >> 2);
    outb(PALETTE_DATA, 0x00 >> 2);

    outb(PALETTE_DATA, 0xB3 >> 2);
    outb(PALETTE_DATA, 0x32 >> 2); // RED
    outb(PALETTE_DATA, 0x27 >> 2);

    outb(PALETTE_DATA, 0x6B >> 2);
    outb(PALETTE_DATA, 0x6C >> 2); // GREEN
    outb(PALETTE_DATA, 0x12 >> 2);

    outb(PALETTE_DATA, 0xE8 >> 2);
    outb(PALETTE_DATA, 0x9D >> 2); // YELLOW
    outb(PALETTE_DATA, 0x34 >> 2);

    outb(PALETTE_DATA,
         0b11010101 >> 2); // This if the best I can correct this color since the VGA color is 6 bit
    outb(PALETTE_DATA, 0x4A >> 2); // DARK_YELLOW
    outb(PALETTE_DATA, 0x11 >> 2);

    outb(PALETTE_DATA, 0xE5 >> 2);
    outb(PALETTE_DATA, 0x9B >> 2); // LIGHT_YELLOW
    outb(PALETTE_DATA, 0x33 >> 2);

    outb(PALETTE_DATA, 0xF0 >> 2);
    outb(PALETTE_DATA, 0xD0 >> 2); // LIGHT_BROWN
    outb(PALETTE_DATA, 0xE0 >> 2);

    outb(PALETTE_DATA, 0xFF >> 2);
    outb(PALETTE_DATA, 0xFF >> 2); // White
    outb(PALETTE_DATA, 0xFF >> 2);

    outb(PALETTE_DATA, 0xE4 >> 2);
    outb(PALETTE_DATA, 0x9B >> 2); // CY
    outb(PALETTE_DATA, 0x33 >> 2);

    outb(PALETTE_DATA, 0x00 >> 2);
    outb(PALETTE_DATA, 0x00 >> 2); // CB
    outb(PALETTE_DATA, 0x00 >> 2);

    outb(PALETTE_DATA, 0x9B >> 2);
    outb(PALETTE_DATA, 0x4A >> 2); // CBR
    outb(PALETTE_DATA, 0x11 >> 2);

    outb(PALETTE_DATA, 0x1C >> 2);
    outb(PALETTE_DATA, 0x92 >> 2); // Hill green
    outb(PALETTE_DATA, 0x17 >> 2);

    outb(PALETTE_DATA, 0x8E >> 2);
    outb(PALETTE_DATA, 0xD4 >> 2); // Bush green
    outb(PALETTE_DATA, 0x28 >> 2);

    outb(PALETTE_DATA, 0x67 >> 2);
    outb(PALETTE_DATA, 0xAF >> 2); // CLOUD BLUE
    outb(PALETTE_DATA, 0xFC >> 2);

    outb(PALETTE_DATA, 0x00 >> 2);
    outb(PALETTE_DATA, 0x00 >> 2); /* Pipe */
    outb(PALETTE_DATA, 0x00 >> 2);
}