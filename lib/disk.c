//
// Created by cheyao on 12.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <kernel/ata.h>
#include <kernel/disk.h>
#include <kernel/ports.h>
#include <stdint.h>
#include <stdio.h>

void read(uint16_t io, uint16_t control) {
    /* Identify command */
    outw(io + DRIVE_SELECT, 0xA0); /* Drive select */

    outw(io + SECTOR_COUNT, 0x00); /* Sector count */
    outw(io + LBA_LOW, 0x00);
    outw(io + LBA_MID, 0x00);
    outw(io + LBA_HIGH, 0x00);
    outw(io + COMMAND_REGISTER, 0xEC);

    uint8_t volatile in = inb(0x1F7); /* Status */

    while ((in & 0x8) != 0x8 && (in & 0x1) == 0x0) {
        in = inb(0x1F7);
        int volatile count = 1000; /* IDK why but someone told me to wait every time */
        while (count--);
    }

    if ((in & 0x1) != 0) {
        printf("[ATA]: Error bit was set");
        return;
    }

    printf("[ATA]: Identifying ATA device\n");

    for (uint16_t volatile i = 0; i < 256; i++)
        *((unsigned short *) 0x7000 + i) = inw(0x1F0); /* get the identify and store to 0x7000 */

    __asm__ __volatile__("mov (0x7060), %eax");
    __asm__ __volatile__("mov (0x7061), %ebx");
    __asm__ __volatile__("mov %ebx, (0x7060)");
    __asm__ __volatile__("mov %eax, (0x7061)");

    printf("[ATA]: Total of %0x addressable sectors\n", *((unsigned int *) 0x7060));
}
