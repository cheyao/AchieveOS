//
// Created by cheyao on 18.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <kernel/cd.h>
#include <kernel/ports.h>
#include <kernel/ata.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

void atapi(Disk d) {
    outb(d.port + DRIVE_SELECT, d.drive_select_command); /* Drive select */

    sleepms(10);

    outb(d.port + SECTOR_COUNT, 0x00); /* Sector count */
    outb(d.port + LBA_LOW, 0x00);
    outb(d.port + LBA_MID, 0x00);
    outb(d.port + LBA_HIGH, 0x00);

    outb(d.port + COMMAND_REGISTER, 0xA1); /* ATAPI Identify command */

    uint16_t in = inb(d.port + COMMAND_REGISTER); /* Status */

    if (in == 0) {
        printf("[ATAPI]: Nothing exists on port %0#x\n", d.port);
        d.type = UNKNOWN;
        return;
    }

    printf("%#x\n", in); /* Prints 0xFF */

    /* Ready/Error */
    while ((in & 0x80) != 0 && (in & 0x1) == 0 && (in & 0x8) == 0) {
        sleepms(10);
        in = inb(d.port + COMMAND_REGISTER);
    }

    if ((in & 0x1) == 1) {
        printf("[ATAPI]: Error bit was set\n");
        return;
    }

    printf("[ATAPI]: Identifying ATA device\n");

    insw(d.port + DATA, ((unsigned short *) 0x7000), 256);

    printf("[ATAPI]: Config: %0x\n", *((uint16_t *) 0x7000));

    reset_ata(d.port);

    if (inb(d.port + SECTOR_COUNT) == 0x01 &&
        inb(d.port + LBA_LOW) == 0x00 &&
        inb(d.port + LBA_MID) == 0x00 &&
        inb(d.port + LBA_HIGH) == 0x00) {
        printf("[ATAPI]: Detected non-packet device\n");
    } else
        printf("[ATAPI]: Detected packet device\n");

    d.type = CDROM;
}

Disk makedisk(uint16_t port, uint8_t ds) {
    Disk d;

    d.port = port;
    d.type = UNDETECTED;
    d.drive_select_command = ds;

    return d;
}
