//
// Created by cheyao on 18.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <kernel/ata.h>
#include <kernel/cd.h>
#include <kernel/ports.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

void identify(Disk *d) {
	int command = 0xEC;

start:
	reset_ata(d->port);
	outb(d->port + DRIVE_SELECT, d->drive_select_command); /* Drive select */

	sleepms(10);

	outb(d->port + SECTOR_COUNT, 0x00); /* Sector count */
	outb(d->port + LBA_LOW, 0x00);
	outb(d->port + LBA_MID, 0x00);
	outb(d->port + LBA_HIGH, 0x00);

	outb(d->port + COMMAND_REGISTER, command); /* Identify command */

	uint8_t in = inb(d->port + COMMAND_REGISTER); /* Status */

	sleepms(10);

	if (in == 0) {
		if (command == 0xEC) {
			command = 0xA1;
			goto start;
		}

		d->type = UNKNOWN;
		return;
	}

	/* Ready/Error */
	while ((in & 0x80) != 0 && (in & 0x1) == 0 && (in & 0x8) == 0) {
		sleepms(10);
		in = inb(d->port + COMMAND_REGISTER);
	}

	if ((in & 0x1) == 1) {
		if (command == 0xEC) {
			command = 0xA1;
			goto start;
		}

		d->type = UNKNOWN;
		return;
	}

	insw(d->port + +DATA, ((unsigned short *) 0x7000), 256); /* Receive identify */

	if (command == 0xEC) {
		if ((d->size = *((uint32_t *) (0x7000 + 60 * 2)))) {
			d->type = HARD_DISK;
			d->protocol = ATA;
			d->removable = false;
			return;
		}

		command = 0xA1;
		goto start;
	}

	if (*((unsigned short *) 0x7000) & 1 << 7)
		d->removable = true;

	if ((*((unsigned short *) 0x7000) >> 14 & 0b11) == 0b10)
		d->protocol = ATAPI;
	else
		d->protocol = ATA;

	switch (*((unsigned short *) 0x7000) >> 8 & 0b11111) {
		case 0x00:
			d->type = DIRECT_ACCESS;
			break;
		case 0x05:
			d->type = CDROM;
			break;
		case 0x07:
			d->type = OPTICAL;
			break;
		default:
			d->type = UNKNOWN;
	}
}

void read_cdrom(Disk *d, uint32_t lba, uint32_t sectors, uint16_t *buffer) {
	uint8_t read_cmd[12] = {0xA8, 0, (lba >> 0x18) & 0xFF, (lba >> 0x10) & 0xFF, (lba >> 0x08) & 0xFF,
	                        (lba >> 0x00) & 0xFF, (sectors >> 0x18) & 0xFF, (sectors >> 0x10) & 0xFF,
	                        (sectors >> 0x08) & 0xFF, (sectors >> 0x00) & 0xFF, 0, 0};

	outb(d->port + DRIVE_SELECT, d->drive_select_command);
	sleepms(10);
	outb(d->port + ERROR_R, 0x00);
	outb(d->port + LBA_MID, 2048 & 0xFF);
	outb(d->port + LBA_HIGH, 2048 >> 8);
	outb(d->port + COMMAND_REGISTER, 0xA0); /* Packet command */
	outsw(d->port + DATA, (uint16_t *) read_cmd, 6);

	uint8_t in;
	/* Poll until BSY is 0 and DRQ is 1 or until ERR is 1 or DF is 1*/
	while (((in = inb(d->port + COMMAND_REGISTER)) & 0x80) != 0 && (in & 0x8) == 0 && (in % 0x1) == 0 &&
	       (in & 0x20) == 0);

	if ((in % 0x1) == 1 && (in & 0x20) == 0x20) {
		printf("[ATAPI]: Error! Error bit was set while reading disk!");
	}
	int size = ((((int) inb(d->port + LBA_HIGH))) << 8) | (int) (inb(d->port + LBA_MID));

	insw(d->port + DATA, buffer, size / 2);

	while (inb(d->port + COMMAND_REGISTER) & 0x88);
}
