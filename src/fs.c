//
// Created by cheyao on 15.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include "fs.h"
#include "ports.h"
#include "stdio.h"

void read(u16int io, u16int control) {
    if (inb(io + 0) == 0xFF) {
        printf("[ATA]: No device!");
        return;
    }

    outb(io + DS, 0xA0); // Drive select

    outb(io + SCR, 0x00); // Sector count
    outb(io + SNR, 0x00);
    outb(io + CLR, 0x00);
    outb(io + CHR, 0x00);
    outb(io + CR, 0xEC);

    u8int volatile in = inb(0x1F7);

    while ((in & 0x8) != 0x8 && (in & 0x1) == 0x0) {
        in = inb(0x1F7);
        int volatile count = 1000;
        while (count--);
    }

    if ((in & 0x1) != 0) {
        printf("[ATA]: Error bit was set");
        return;
    }

    printf("[ATA]: Identifying ATA device\n");

    for (u16int volatile i = 0; i < 256; i++)
        *((unsigned short *) 0x7000 + i) = inw(0x1F0);

    printf("[ATA]: Total of %0#x addressable sectors\n", *((unsigned int *) 0x7060));
}
