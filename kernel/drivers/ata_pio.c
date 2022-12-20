//
// Created by cheyao on 20.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//
#include <kernel/ata.h>
#include <drivers/ata_pio.h>

static void ata_io_wait(const uint8_t p) {
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
}

bool identify_disk(uint16_t port) {
	reset_ata(port);

	outb(port + DRIVE_SELECT, port); /* Drive select */

	ata_io_wait(port);

	outb(port + SECTOR_COUNT, 0x00); /* Sector count */
	outb(port + LBA_LOW, 0x00);
	outb(port + LBA_MID, 0x00);
	outb(port + LBA_HIGH, 0x00);

	outb(port + COMMAND_REGISTER, 0xEC); /* Identify command */

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

void write_disk(const uint16_t port, const uint8_t drive_select, const uint32_t lba, const uint32_t sectors,
                uint16_t *buffer) {
	reset_ata(port);
	outb(port + DRIVE_SELECT, drive_select | (1 << 6)); // drive select with lba bit set
	ata_io_wait(port);
	outb(port + SECTOR_COUNT, sectors);
	outb(port + LBA_LOW, (lba & 0x000000ff) >> 0);
	outb(port + LBA_MID, (lba & 0x0000ff00) >> 8);
	outb(port + LBA_HIGH, (lba & 0x00ff0000) >> 16);
	outb(port + COMMAND_REGISTER, 0x30);  // Write disk command

	ata_io_wait(port);

	for (uint32_t i = 0; i < sectors; i++) {
		ata_io_wait(port);

		while (1) {
			uint8_t status = inb(port + COMMAND_REGISTER);

			if (!(status & 0x80) && (status & 0x08))
				break;
			else if (status & 0x01)
				return;

			ata_io_wait(port);
		}

		for (int j = 0; j < 256; j++, buffer++)
			outw(port + DATA, *buffer);
	}

	ata_io_wait(port);
	outb(port + DRIVE_SELECT, 0xA0);
	ata_io_wait(port);
	outb(port + COMMAND_REGISTER, 0xE7);  // Flush cache
	ata_io_wait(port);
}
