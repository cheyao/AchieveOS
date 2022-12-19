//
// Created by cheyao on 14.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//
#include <kernel/ports.h>
#include <kernel/ata.h>
#include <kernel/elf.h>
#include <stdint.h>
#include <stddef.h>

static uint16_t drive_port;
static uint16_t drive_select;

static void ata_io_wait(uint8_t p);

void read_disk(uint32_t lba, uint32_t sectors, uint16_t *buffer);

bool identify_disk(void);

void putchar(char c);

void puts(const char *restrict str);

size_t main(void) {
	drive_port = BUS_PRIMARY;
	drive_select = 0xA0;
	if (!identify_disk())
		goto end;
	drive_select = 0xB0;
	if (!identify_disk())
		goto end;
	drive_port = BUS_SECONDARY;
	if (!identify_disk())
		goto end;
	drive_select = 0xA0;
	if (!identify_disk())
		goto end;

	end:
	puts("Hello world from the drive!\n");

	read_disk(0x400, 1, (uint16_t *) 0x1000);

	ELF_header *header = (ELF_header *) 0x1000;

	if (header->e_ident[0] == 0x7F &&
	    header->e_ident[1] == 'E' &&
	    header->e_ident[2] == 'L' &&
	    header->e_ident[3] == 'F') {
		puts("ELF file recognised\n");
		if (header->e_ident[4] != 2)
			puts("Error! Not 64 bit!\n");

		if (header->e_type != 2)
			puts("Error! Not executable!\n");

		read_disk(0x400 + header->e_phoff / 512, 2, (uint16_t *) 0x1200);

		Program_header *pheader = (Program_header *) (0x1200 + header->e_phoff % 512);

		for (size_t i = 0; i < header->e_phnum; i++) {
			if (pheader->p_type == 1)
				read_disk(0x400 + pheader->p_offset / 512, pheader->p_filesz / 512 + 1, (uint16_t *) pheader->p_vaddr);

			if (pheader->p_filesz < pheader->p_memsz)
				for (size_t j = 0; i < pheader->p_memsz - pheader->p_filesz; i++)
					((char *) pheader->p_vaddr + pheader->p_filesz)[j] = 0;

			pheader++;
		}

		return header->e_entry;
	} else
		puts("Error! Not a ELF file\n");
	return 0;
}

static void ata_io_wait(const uint8_t p) {
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
}

void putchar(const char c) {
	while (!(inb(0x378 + 1) & 0x80))
		ata_io_wait(drive_port);

	outb(0x378, c);

	unsigned char control = inb(0x37A);
	outb(0x378 + 2, control | 1);
	ata_io_wait(drive_port);
	outb(0x378 + 2, control);
	outb(0xe9, c);

	while (!(inb(0x378 + 1) & 0x80))
		ata_io_wait(drive_port);
}

void puts(const char *restrict str) {
	for (size_t i = 0; str[i] != 0; i++)
		putchar(str[i]);
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

void read_disk(const uint32_t lba, const uint32_t sectors, uint16_t *buffer) {
	reset_ata(drive_port);
	outb(drive_port + DRIVE_SELECT, drive_select | (1 << 6)); // drive select with lba bit set
	ata_io_wait(drive_port);
	outb(drive_port + SECTOR_COUNT, sectors);
	outb(drive_port + LBA_LOW, (lba & 0x000000ff) >> 0);
	outb(drive_port + LBA_MID, (lba & 0x0000ff00) >> 8);
	outb(drive_port + LBA_HIGH, (lba & 0x00ff0000) >> 16);
	outb(drive_port + COMMAND_REGISTER, 0x20);  // Read disk command

	ata_io_wait(drive_port);

	for (uint32_t i = 0; i < sectors; i++) {
		ata_io_wait(drive_port);

		int timeout = 1000;
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

		insw(drive_port + DATA, buffer + i * 256, 256);
	}

	ata_io_wait(drive_port);

	outb(drive_port + DRIVE_SELECT, 0xA0);

	ata_io_wait(drive_port);

	outb(drive_port + COMMAND_REGISTER, 0xE7);  // Flush cache

	ata_io_wait(drive_port);
}
