//
// Created by cheyao on 15.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include "fs.h"
#include "ports.h"
#include "stdio.h"

void read(void) {
    outb(0x1F0, 0xA0); // Drive select
    outb(0x1F2, 0x00); // Sector count
    outb(0x1F3, 0x00);
    outb(0x1F4, 0x00);
    outb(0x1F5, 0x00);
    outb(0x1F6, 0xEC);

    u8int in = inb(0x1F7);

    printf("%#0hhx", in);
    while (((in = inb(0x1F7)) & 0x8) != 0x8 || (in & 0x0) == 0x0);
    printf("%#0hhx", in); // Not printing

}
