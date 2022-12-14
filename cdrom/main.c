//
// Created by cheyao on 12.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <kernel/ata.h>
#include <kernel/ports.h>
#include <stdint.h>
#include <hedley.h>

#define BUFFER 0xB8000

int port = 0x00;
int drive_select = 0x00;
int drive_port = 0x00;
int drive_drive_select = 0x00;

void read_cdrom(uint32_t lba, uint32_t sectors, uint16_t *buffer);

void write_disk(uint32_t lba, uint32_t sectors, uint16_t *buffer);

static void ata_io_wait(uint8_t p);

bool identify(void);

bool ata_identify(void);

#ifdef DEBUG

void puts_parallel(const char *restrict string) {
	for (int i = 0; string[i] != 0; i++) {
		outb(0xE9, string[i]);

		while (!(inb(0x379) & (1 << 7)))
			ata_io_wait(port);

		outb(0x378, string[i]);

		uint8_t lControl = inb(0x37A);
		outb(0x37A, lControl | 1);
		ata_io_wait(port);
		outb(0x37A, lControl);

		while (!(inb(0x379) & (1 << 7)))
			ata_io_wait(port);
	}
}

#else
#define puts_parallel(str)
#endif

bool strcmp(const char a[], const char b[]) {
	for (int i = 0; a[i] != 0; i++)
		if (a[i] != b[i])
			return false;

	return true;
}

void main(void) {
	// Detecting CD-ROM
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
	read_cdrom(0x10, 1, (uint16_t *) 0x100000);
	read_cdrom(*((uint32_t *) 0x10009E), 1, (uint16_t *) 0x100000);

	// Detecting hard disk
	{
		drive_port = BUS_PRIMARY;
		drive_drive_select = 0xA0;
		if (!ata_identify())
			goto eend;
		drive_drive_select = 0xB0;
		if (!ata_identify())
			goto eend;
		drive_port = BUS_SECONDARY;
		if (!ata_identify())
			goto eend;
		drive_drive_select = 0xA0;
		if (!ata_identify())
			goto eend;
	}
	eend:
	{
		uintptr_t addr = 0x100000;
		addr += *((uint8_t *) addr);
		addr += *((uint8_t *) addr);
		while (*((uint8_t *) addr) != 0) {
			// If the file is bootsect.bin
			if (HEDLEY_UNLIKELY(strcmp("bootsect.bin", (const char *) (addr + 33)) != 0)) {
				read_cdrom(*((uint32_t *) (addr + 2)), 1, (uint16_t *) 0x100800);

				write_disk(1, 1, (uint16_t *) 0x100800);
			}

			// If the file is kernel.bin
			if (HEDLEY_UNLIKELY(strcmp("kernel.bin", (const char *) (addr + 33)) != 0)) {
				read_cdrom(*((uint32_t *) (addr + 2)), *((uint32_t *) (addr + 10)) / 2048 + 1, (uint16_t *) 0x100800);

				write_disk(2, *((uint32_t *) (addr + 10)) / 2048 + 1, (uint16_t *) 0x100800);
			}

			addr += *((uint8_t *) addr);
		}
	}

	uint8_t good = 0x02;
	while (good & 0x02)
		good = inb(0x64);
	outb(0x64, 0xFE);

	__asm__ __volatile__ ("int $0");
}

bool identify(void) {
	reset_ata(port);

	outb(port + DRIVE_SELECT, drive_select); /* Drive select */

	ata_io_wait(port);

	outb(port + SECTOR_COUNT, 0x00); /* Sector count */
	outb(port + LBA_LOW, 0x00);
	outb(port + LBA_MID, 0x00);
	outb(port + LBA_HIGH, 0x00);

	outb(port + COMMAND_REGISTER, 0xA1); /* Identify command */

	ata_io_wait(port);

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

bool ata_identify(void) {
	reset_ata(drive_port);

	outb(drive_port + DRIVE_SELECT, drive_select); /* Drive select */

	ata_io_wait(drive_port);

	outb(drive_port + SECTOR_COUNT, 0x00); /* Sector count */
	outb(drive_port + LBA_LOW, 0x00);
	outb(drive_port + LBA_MID, 0x00);
	outb(drive_port + LBA_HIGH, 0x00);

	outb(drive_port + COMMAND_REGISTER, 0xEC); /* Identify command */

	ata_io_wait(drive_port);

	int timeout = 100;
	while (1) {
		uint8_t status = inb(drive_port + COMMAND_REGISTER);
		if ((status & 0x01)) return 1;
		if (timeout-- < 0) return 1;
		if (!(status & 0x80) && (status & 0x08)) break;
	}

	insw(drive_port + DATA, ((unsigned short *) 0x7000), 256); /* Receive identify */

	return 0;
}

static void ata_io_wait(const uint8_t p) {
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
}

void read_cdrom(uint32_t lba, uint32_t sectors, uint16_t *buffer) {
	puts_parallel("Reading from CR-ROM\n");

	uint8_t read_cmd[12] = {0xA8, 0,
	                        (lba >> 0x18) & 0xFF, (lba >> 0x10) & 0xFF, (lba >> 0x08) & 0xFF, (lba >> 0x00) & 0xFF,
	                        (sectors >> 0x18) & 0xFF, (sectors >> 0x10) & 0xFF, (sectors >> 0x08) & 0xFF,
	                        (sectors >> 0x00) & 0xFF,
	                        0, 0};

	outb(port + DRIVE_SELECT, drive_select);
	ata_io_wait(port);
	outb(port + ERROR_R, 0x00);
	outb(port + LBA_MID, 2048 & 0xFF);
	outb(port + LBA_HIGH, 2048 >> 8);
	outb(port + COMMAND_REGISTER, 0xA0); /* Packet command */

	while (1) {
		uint8_t status = inb(port + COMMAND_REGISTER);
		if ((status & 0x01)) {
			puts_parallel("err 1!");
			return;
		}
		if (!(status & 0x80) && (status & 0x08)) break;
	}

	outsw(port + DATA, (uint16_t *) read_cmd, 6);

	for (uint32_t i = 0; i < sectors; i++) {
		while (1) {
			uint8_t status = inb(port + COMMAND_REGISTER);
			if ((status & 0x01)) {
				puts_parallel("err!");
				return;
			}
			if (!(status & 0x80) && (status & 0x08)) break;
		}

		int size = inb(port + LBA_HIGH) << 8
		           | inb(port + LBA_MID);

		insw(port + DATA, (uint16_t *) ((uint8_t *) buffer + i * 0x800), size / 2);
	}
}

void write_disk(uint32_t lba, uint32_t sectors, uint16_t *buffer) {
	puts_parallel("Writing to hard disk\n");

	outb(drive_port + DRIVE_SELECT, 0xA0);
	ata_io_wait(drive_port);
	outb(drive_port + SECTOR_COUNT, sectors);
	outb(drive_port + LBA_LOW, (uint8_t) lba);
	outb(drive_port + LBA_MID, (uint8_t) (lba >> 8));
	outb(drive_port + LBA_HIGH, (uint8_t) (lba >> 16));
	outb(drive_port + COMMAND_REGISTER, 0x30); // Write disk command

	ata_io_wait(drive_port);

	for (uint32_t i = 0; i < sectors; i++) {
		ata_io_wait(drive_port);

		while (1) {
			uint8_t status = inb(drive_port + COMMAND_REGISTER);
			if ((status & 0x01)) {
				puts_parallel("err1!");
				return;
			}
			if (!(status & 0x80) && (status & 0x08)) break;
		}

		for (int j = 0; j < 256; j++, buffer++) {
			outw(drive_port + DATA, *buffer);
		}

		ata_io_wait(drive_port);
	}
}
