//
// Created by cheyao on 12.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <kernel/ata.h>
#include <kernel/ports.h>
#include <stdint.h>

#define BUFFER 0xB8000

int port = 0x00;
int drive_select = 0x00;

void read_cdrom(uint32_t lba, uint32_t sectors, uint16_t *buffer);

static void ata_io_wait(void);

int identify(void);

void puts_parallel(const char *restrict string);

void main(void) {
	{
		port = BUS_PRIMARY;
		drive_select = 0xA0;
		if (!identify())
			goto end;
		drive_select = 0xB0;
		if (!identify())
			goto end;
		port = BUS_SECONDARY;
		if (!identify())
			goto end;
		drive_select = 0xA0;
		if (!identify())
			goto end;
	}
	end:
	puts_parallel("Hi parallel");
	read_cdrom(0x10, 5, (uint16_t *) 0x100000);
}

int identify(void) {
	reset_ata(port);

	outb(port + DRIVE_SELECT, drive_select); /* Drive select */

	ata_io_wait();

	outb(port + SECTOR_COUNT, 0x00); /* Sector count */
	outb(port + LBA_LOW, 0x00);
	outb(port + LBA_MID, 0x00);
	outb(port + LBA_HIGH, 0x00);

	outb(port + COMMAND_REGISTER, 0xA1); /* Identify command */

	ata_io_wait();

	int timeout = 100;
	while (1) {
		uint8_t status = inb(port + COMMAND_REGISTER);
		if ((status & 0x01)) return 1;
		if (timeout-- < 0) return 1;
		if (!(status & 0x80) && (status & 0x08)) break;
	}

	insw(port + DATA, ((unsigned short *) 0x7000), 256); /* Receive identify */

	return 0;
}

static void ata_io_wait(void) {
	inb(port + ALTERNATE_STATUS + CONTROL);
	inb(port + ALTERNATE_STATUS + CONTROL);
	inb(port + ALTERNATE_STATUS + CONTROL);
	inb(port + ALTERNATE_STATUS + CONTROL);
}

void read_cdrom(uint32_t lba, uint32_t sectors, uint16_t *buffer) {
	uint8_t read_cmd[12] = {0xA8, 0,
	                        ((lba) >> 0x18) & 0xFF, ((lba) >> 0x10) & 0xFF, ((lba) >> 0x08) & 0xFF,
	                        ((lba) >> 0x00) & 0xFF,
	                        (sectors >> 0x18) & 0xFF, (sectors >> 0x10) & 0xFF, (sectors >> 0x08) & 0xFF,
	                        (sectors >> 0x00) & 0xFF,
	                        0, 0};

	outb(port + DRIVE_SELECT, 0xA0);
	ata_io_wait();
	outb(port + ERROR_R, 0x00);
	outb(port + LBA_MID, 2048 & 0xFF);
	outb(port + LBA_HIGH, 2048 >> 8);
	outb(port + COMMAND_REGISTER, 0xA0); /* Packet command */

	int timeout = 100;
	while (1) {
		uint8_t status = inb(port + COMMAND_REGISTER);
		if ((status & 0x01)) {
			puts_parallel("err!");
			return;
		}
		if (timeout-- < 0) {
			puts_parallel("Timeout 1!");
			return;
		}
		if (!(status & 0x80) && (status & 0x08)) break;
	}

	outsw(port + DATA, (uint16_t *) read_cmd, 6);

	timeout = 100;
	while (1) {
		uint8_t status = inb(port + COMMAND_REGISTER);
		if ((status & 0x01)) {
			puts_parallel("err!");
			return;
		}
		if (timeout-- < 0) {
			puts_parallel("Timeout 2!");
			return;
		}
		if (!(status & 0x80) && (status & 0x08)) break;
	}

	int size = inb(port + LBA_HIGH) << 8
	           | inb(port + LBA_MID);

	insw(port + DATA, (uint16_t *) ((uint8_t *) buffer), size / 2);

	/* Wait until the ports are clean*/
	while (1) {
		uint8_t status = inb(port + COMMAND_REGISTER);
		if ((status & 0x01)) {
			puts_parallel("err!");
			return;
		}
		if (!(status & 0x80) && (status & 0x08)) break;
	}
}

void puts_parallel(const char *restrict string) {
	for (int i = 0; string[i] != 0; i++) {
		outb(0xE9, string[i]);

		while (!(inb(0x379) & (1 << 7)))
			ata_io_wait();

		outb(0x378, string[i]);

		uint8_t lControl = inb(0x37A);
		outb(0x37A, lControl | 1);
		ata_io_wait();
		outb(0x37A, lControl);

		while (!(inb(0x379) & (1 << 7)))
			ata_io_wait();
	}
}
