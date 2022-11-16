//
// Created by cheyao on 15.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include "fs.h"
#include "ports.h"
#include "stdio.h"

void read(int port) {
    if (inb(port + 0) == 0xFF) {
        printf("No device!");
        return;
    }

    outb(port + 0, 0xA0); // Drive select
    outb(port + 2, 0x00); // Sector count
    outb(port + 3, 0x00);
    outb(0x1F4, 0x00);
    outb(0x1F5, 0x00);
    outb(0x1F6, 0xEC);

    u8int volatile in = inb(0x1F7);

    printf("%#0hhx", in); // Fist value

    while ((in & 0x8) != 0x8 && (in & 0x1) == 0x0) {
        in = inb(0x1F7);
        int volatile count = 1000;
        while (count--);
    }

    printf("%#0hhx", in); // Not printing

}
