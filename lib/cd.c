//
// Created by cheyao on 18.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <kernel/cd.h>
#include <kernel/ports.h>
#include <kernel/ata.h>
#include <stdio.h>

void atapi(int IO, int control) {
    outb(IO + COMMAND_REGISTER, 0x08);
    // reset_ata(IO);
    //if (inb(IO + SECTOR_COUNT) == 0x01 &&
    //    inb(IO + LBA_LOW) == 0x00 &&
    //    inb(IO + LBA_MID) == 0x00 &&
    //    inb(IO + LBA_HIGH) == 0x00) {
    //    printf("");
    //}
}
