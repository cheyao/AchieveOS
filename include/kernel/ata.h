//
// Created by cheyao on 18.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _ATA_H
#define _ATA_H

#include <kernel/ports.h>
#include <stdint.h>

#define reset_ata(port) outb(port + COMMAND_REGISTER, 0x08)
#define insw(port, buffer, count) __asm__ ("cld; rep; insw" :: "D" (buffer), "d" (port), "c" (count))

#define BUS_PRIMARY 0x1F0
#define BUS_SECONDARY 0x170
#define CONTROL_PRIMARY 0x3f6
#define CONTROL_SECONDARY 0x376

#define DATA 0
#define ERROR 1
#define SECTOR_COUNT 2
#define LBA_LOW 3
#define LBA_MID 4
#define LBA_HIGH 5
#define DRIVE_SELECT 6
#define COMMAND_REGISTER 7

#define ALTERNATE_STATUS 0
#define DRIVE_CONTROL 1

typedef struct {
    enum DISK_TYPE {
        UNDETECTED, UNKNOWN, CDROM, HARD_DISK
    } type;
    uint8_t port;
    uint16_t drive_select_command;
} Disk;

#endif //_ATA_H
