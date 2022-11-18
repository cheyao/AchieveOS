//
// Created by cheyao on 18.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _ATA_H
#define _ATA_H

#include <kernel/ports.h>

#define reset_ata(port) outb(port + COMMAND_REGISTER, 0x08)

#define BUS_PRIMARY 0x1F0
#define BUS_SECONDARY 0x170
#define CONTROL_PRIMARY 0x3f0
#define CONTROL_SECONDARY 0x370

#define DATA 0
#define ERROR 1
#define SECTOR_COUNT 2
#define LBA_LOW 3
#define LBA_MID 4
#define LBA_HIGH 5
#define DRIVE_SELECT 6
#define COMMAND_REGISTER 7

#endif //_ATA_H
