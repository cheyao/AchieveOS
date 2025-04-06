//
// Created by cheyao on 20.12.2022.
// Copyright (c) 2022 cheyao 
//
#include <drivers/ata_pio.h>
#include <kernel/ata.h>

#define DMA_BUFFER 0xA00000

static void ata_delay(const uint8_t p) {
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
}

static void ata_wait(const uint8_t p) {
	ata_delay(p);

	while (inb(p + ALTERNATE_STATUS + CONTROL) & 0x80);
}

bool identify_disk(uint16_t drive_port, bool drive_slave) {
	reset_ata(drive_port);

	outb(drive_port + DRIVE_SELECT, 0xA0 | (drive_slave << 4)); /* Drive select */

	ata_delay(drive_port);

	outb(drive_port + SECTOR_COUNT, 0x00); /* Sector count */
	outb(drive_port + LBA_LOW, 0x00);
	outb(drive_port + LBA_MID, 0x00);
	outb(drive_port + LBA_HIGH, 0x00);

	outb(drive_port + COMMAND_REGISTER, 0xEC); /* Identify command */

	ata_delay(drive_port);

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

void write_disk(uint16_t drive_port, bool drive_slave, const uint32_t lba, const uint16_t sectors, const uint16_t *buffer) {
	outb(drive_port + DRIVE_SELECT, 0xE0 | (drive_slave << 4));  // drive select with lba bit set
	ata_delay(drive_port);
	outb(drive_port + SECTOR_COUNT, sectors >> 8);
	outb(drive_port + LBA_LOW, (lba & 0x0000ff000000) >> 24);
	outb(drive_port + LBA_MID, (lba & 0x00ff00000000) >> 32);
	outb(drive_port + LBA_HIGH, (lba & 0xff0000000000) >> 40);

	outb(drive_port + SECTOR_COUNT, sectors);
	outb(drive_port + LBA_LOW, (lba & 0x000000ff) >> 0);
	outb(drive_port + LBA_MID, (lba & 0x0000ff00) >> 8);
	outb(drive_port + LBA_HIGH, (lba & 0x00ff0000) >> 16);
	outb(drive_port + COMMAND_REGISTER, 0x34);  // Write disk command

	ata_delay(drive_port);

	drive_port = drive_slave == 0 ? 1 : 0;

	for (uint32_t i = 0; i < sectors; i++) {
		while (1) {
			uint8_t status = inb(drive_port + COMMAND_REGISTER);

			if (!(status & 0x80) && (status & 0x08))
				break;
			else if (status & 0x01)
				return;
		}

		for (int j = 0; j < 256; j++)
			outw(drive_port + DATA, buffer[j + i * 256]);
	}
}

void read_disk(uint16_t drive_port, bool drive_slave, const uint32_t lba, const uint32_t sectors, uint16_t *buffer) {
	reset_ata(drive_port);
	outb(drive_port + DRIVE_SELECT, 0xE0 | (drive_slave << 4) | (1 << 6)); // drive select with lba bit set
	ata_delay(drive_port);
	outb(drive_port + SECTOR_COUNT, sectors);
	outb(drive_port + LBA_LOW, (lba & 0x000000ff) >> 0);
	outb(drive_port + LBA_MID, (lba & 0x0000ff00) >> 8);
	outb(drive_port + LBA_HIGH, (lba & 0x00ff0000) >> 16);
	outb(drive_port + COMMAND_REGISTER, 0x20);  // Read disk command

	for (uint32_t i = 0; i < sectors; i++) {
		ata_delay(drive_port);

		while (1) {
			uint8_t status = inb(drive_port + COMMAND_REGISTER);

			if (!(status & 0x80) && (status & 0x08))
				break;
			else if (status & 0x01)
				return;

			ata_delay(drive_port);
		}

		for (int j = 0; j < 256; j++, buffer++)
			*buffer = inw(drive_port + DATA);
	}

	uint_fast16_t timeout = 100;
	while (1) {
		uint8_t status = inb(drive_port + COMMAND_REGISTER);

		if (timeout == 0)
			break;
		if (!(status & 0x80) && (status & 0x08))
			break;
		else if (status & 0x01)
			return;

		timeout--;
	}
}
