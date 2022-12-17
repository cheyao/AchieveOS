//
// Created by cheyao on 12.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <hedley.h>
#include <kernel/ata.h>
#include <kernel/ports.h>
#include <stdint.h>
#include <stdbool.h>


#define BUFFER 0xB8000

int port = 0x00;
int drive_select = 0x00;
int drive_port = 0x00;
int drive_drive_select = 0x00;

int read_cdrom(uint32_t lba, uint32_t sectors, uint16_t *buffer);

void write_disk(uint32_t lba, uint32_t sectors, uint16_t *buffer);

static void ata_io_wait(uint8_t p);

bool identify_cdrom(void);

bool identify_disk(void);

void eject_cdrom(void);

/**
 * It sends a string to the parallel port, and then waits for the printer to acknowledge that it has received the data
 *
 * @param string The string to print.
 */
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

/**
 * It returns true if the two strings are equal, and false otherwise
 *
 * @param a The first string to compare.
 * @param b The base address of the string.
 *
 * @return a boolean value.
 */
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
		if (!identify_cdrom())
			goto end;
		drive_select = 0xB0;
		if (!identify_cdrom())
			goto end;
		port = BUS_SECONDARY;
		if (!identify_cdrom())
			goto end;
		drive_select = 0xA0;
		if (!identify_cdrom())
			goto end;
	}
	end:
	// Detecting hard disk
	{
		drive_port = BUS_PRIMARY;
		drive_drive_select = 0xA0;
		if (!identify_disk())
			goto eend;
		drive_drive_select = 0xB0;
		if (!identify_disk())
			goto eend;
		drive_port = BUS_SECONDARY;
		if (!identify_disk())
			goto eend;
		drive_drive_select = 0xA0;
		if (!identify_disk())
			goto eend;
	}
	eend :
	{
		// First initialize disk GPT portions
		// Partition 1 - Boot sector

		// Partition 2 - APFS
		*((uint64_t *) (0x100000 + 0x80 + 0x00)) = 0xACEC4365300011AAULL; // Type GUID
		*((uint64_t *) (0x100000 + 0x80 + 0x08)) = 0x11AA00007C3457EFULL;
		// Partition GUID
		volatile uint32_t r;
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x10)) = r;  // GUID upper half
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x80 + 0x18)) = r;  // GUID lower half
		*((uint64_t *) (0x100000 + 0x80 + 0x20)) = 0x800;
		*((uint64_t *) (0x100000 + 0x80 + 0x28)) = 0x800;

		// Partition 3 - SWAP

		// write_disk(3, 1, (uint16_t *) 0x100000);
		// write_disk(*((uint64_t *) ((uint16_t *) 0x7000 + 100)) - 33, 1, (uint16_t *) 0x100000);

		// Partition Table Header
		// MBR included in bootsect.bin
		*((uint64_t *) (0x100000 + 0x00)) = 0x5452415020494645ULL;  // 0x00 "EFI PART"
		*((uint32_t *) (0x100000 + 0x08)) = 0x00010000U; // Revision 1.0
		*((uint32_t *) (0x100000 + 0x0C)) = 0x0000005CU; // Header size
		*((uint32_t *) (0x100000 + 0x10)) = 0x00000000U; // CRC - calculate last, put 0
		*((uint32_t *) (0x100000 + 0x14)) = 0x00000000U; // Reserved
		*((uint64_t *) (0x100000 + 0x18)) = 0x0000000000000000001ULL;  // Current LBA
		*((uint64_t *) (0x100000 + 0x20)) = 0x0000000000000000001ULL;  // Backup  LBA :P no backup atm
		*((uint64_t *) (0x100000 + 0x28)) = 0x0000000000000000022ULL;  // First usable LBA for partitions
		*((uint64_t *) (0x100000 + 0x30)) = *((uint64_t *) ((uint16_t *) 0x7000 + 100)) -
		                                    34;  // Last usable LBA, using value returned from identify
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x38)) = r;  // GUID
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x3C)) = r;
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x40)) = r;
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x44)) = r;
		*((uint32_t *) (0x100000 + 0x48)) = 0x00000002U;  // Starting LBA of array of partition entries
		*((uint32_t *) (0x100000 + 0x50)) = 0x00000180U;  // Number of partition entries in array
		*((uint32_t *) (0x100000 + 0x54)) = 0x00000080U;  // Size of a single partition entry
		// CRC checksum

		/* Filling the rest of the partition with zeros. */
		for (uint_fast16_t i = 0; i < 420; i++)
			*((uint8_t *) (0x100000 + 0x5C + i)) = 0x0;


		// GPT LBA 2
		write_disk(2, 1, (uint16_t *) 0x100000);
		asm volatile ("xchg %bx, %bx");
		// passed: 0x3ffff binary: 0b00000011 0b11111111 0b11111111
		// got:    0xfbd0e binary: 0b00001111 0b10111101 0b00001110
		// 00039372670e[HD    ] logical address out of bounds (1031438/262144) - aborting command
		write_disk(*((uint64_t *) 0x70C8) - 1, 1, (uint16_t *) 0x100000);

		// Move files
		// Dir table is in 0x100000
		read_cdrom(0x10, 1, (uint16_t *) 0x100000);
		read_cdrom(*((uint32_t *) 0x10009E), 1, (uint16_t *) 0x100000);

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

				write_disk(4, *((uint32_t *) (addr + 10)) / 2048 + 1, (uint16_t *) 0x100800);
			}

			addr += *((uint8_t *) addr);
		}
	}

	eject_cdrom();

	uint8_t good = 0x02;
	while (good & 0x02)
		good = inb(0x64);
	outb(0x64, 0xFE);

	__asm__ __volatile__("int $0");
}

static void ata_io_wait(const uint8_t p) {
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
}

bool identify_disk(void) {
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

void write_disk(const uint32_t lba, const uint32_t sectors, uint16_t *buffer) {
	outb(drive_port + DRIVE_SELECT, drive_drive_select);
	ata_io_wait(drive_port);
	outb(drive_port + SECTOR_COUNT, sectors);
	outb(drive_port + LBA_LOW, (lba & 0x000000ff) >> 0);
	outb(drive_port + LBA_MID, (lba & 0x0000ff00) >> 8);
	outb(drive_port + LBA_HIGH, (lba & 0x00ff0000) >> 16);
	outb(drive_port + COMMAND_REGISTER, 0x30);  // Write disk command

	ata_io_wait(drive_port);

	for (uint32_t i = 0; i < sectors; i++) {
		ata_io_wait(drive_port);

		while (1) {
			uint8_t status = inb(drive_port + COMMAND_REGISTER);
			if (status & 0x01)
				return;
			if (!(status & 0x80) && (status & 0x08))
				break;
		}

		for (int j = 0; j < 256; j++, buffer++)
			outw(drive_port + DATA, *buffer);

		ata_io_wait(drive_port);
	}

	outb(drive_port + DRIVE_SELECT, 0xA0);

	ata_io_wait(drive_port);

	outb(drive_port + COMMAND_REGISTER, 0xE7);  // Flush cache

	ata_io_wait(drive_port);
}

bool identify_cdrom(void) {
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

int read_cdrom(uint32_t lba, uint32_t sectors, uint16_t *buffer) {
	volatile uint8_t read_cmd[12] = {0xA8, 0,
	                                 (lba >> 0x18) & 0xFF, (lba >> 0x10) & 0xFF, (lba >> 0x08) & 0xFF,
	                                 (lba >> 0x00) & 0xFF,
	                                 (sectors >> 0x18) & 0xFF, (sectors >> 0x10) & 0xFF, (sectors >> 0x08) & 0xFF,
	                                 (sectors >> 0x00) & 0xFF,
	                                 0, 0};

	outb(port + DRIVE_SELECT, drive_select);
	ata_io_wait(port);
	outb(port + ERROR_R, 0x00);
	outb(port + LBA_MID, 2048 & 0xFF);
	outb(port + LBA_HIGH, 2048 >> 8);
	outb(port + COMMAND_REGISTER, 0xA0); /* Packet command */
	ata_io_wait(port);

	while (1) {
		uint8_t status = inb(port + COMMAND_REGISTER);
		if ((status & 0x01) == 1)
			return 1;
		if (!(status & 0x80) && (status & 0x08))
			break;
		ata_io_wait(port);
	}

	outsw(port + DATA, (uint16_t *) read_cmd, 6);

	for (uint32_t i = 0; i < sectors; i++) {
		while (1) {
			uint8_t status = inb(port + COMMAND_REGISTER);
			if (status & 0x01)
				return 1;
			if (!(status & 0x80) && (status & 0x08))
				break;
		}

		int size = inb(port + LBA_HIGH) << 8 | inb(port + LBA_MID);

		insw(port + DATA, (uint16_t *) ((uint8_t *) buffer + i * 0x800), size / 2);
	}

	return 0;
}

void eject_cdrom(void) {
	volatile uint8_t read_cmd[12] = {0x1B, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0};

	outb(port + DRIVE_SELECT, drive_select);
	ata_io_wait(port);
	outb(port + ERROR_R, 0x00);
	outb(port + LBA_MID, 2048 & 0xFF);
	outb(port + LBA_HIGH, 2048 >> 8);
	outb(port + COMMAND_REGISTER, 0xA0); /* Packet command */

	while (1) {
		uint8_t status = inb(port + COMMAND_REGISTER);
		if (HEDLEY_UNLIKELY(status & 0x01))
			return;
		if (!(status & 0x80) && (status & 0x08))
			break;
	}

	outsw(port + DATA, (uint16_t *) read_cmd, 6);
}
