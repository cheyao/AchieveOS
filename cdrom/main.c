//
// Created by cheyao on 12.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <hedley.h>
#include <kernel/ata.h>
#include <kernel/ports.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define BUFFER 0xB8000

static uint16_t port = 0x00;
static uint16_t drive_select = 0x00;
static uint16_t drive_port = 0x00;
static uint16_t drive_slave = 0x00;

int read_cdrom(uint32_t lba, uint32_t sectors, uint16_t *buffer);

void write_disk(uint64_t lba, uint16_t sectors, uint16_t *buffer);

static void ata_io_wait(uint8_t p);

bool identify_cdrom(void);

bool identify_disk(void);

void eject_cdrom(void);

uint32_t CRC32(const uint8_t data[], size_t data_length);

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
	puts("Entered CDROM burner\n");
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
	puts("CDROM finished detecting\n");
	// Detecting hard disk
	{
		drive_port = BUS_PRIMARY;
		drive_slave = 0;
		if (!identify_disk())
			goto eend;
		drive_slave = 1;
		if (!identify_disk())
			goto eend;
		drive_port = BUS_SECONDARY;
		if (!identify_disk())
			goto eend;
		drive_slave = 0;
		if (!identify_disk())
			goto eend;
	}
	eend:
	puts("Hard disk finished detecting\n");
	{
		volatile uint32_t r;

		// First initialize disk GPT portions
		// Partition 1 - Second stage boot sector
		*((uint64_t *) (0x100000 + 0x00)) = 0x7553734949464555ULL; // Type GUID
		*((uint64_t *) (0x100000 + 0x08)) = 0x2164726148726570ULL;
		__asm__ __volatile__("rdrand %0": "=a" (r)); // Partition GUID
		*((uint32_t *) (0x100000 + 0x10)) = r;
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x14)) = r;
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x18)) = r;
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x1C)) = r;
		*((uint64_t *) (0x100000 + 0x21)) = 0x20;
		*((uint64_t *) (0x100000 + 0x28)) = 0x7FF;
		*((uint8_t *) (0x100000 + 0x38)) = 'B';
		*((uint8_t *) (0x100000 + 0x3A)) = 'O';
		*((uint8_t *) (0x100000 + 0x3C)) = 'O';
		*((uint8_t *) (0x100000 + 0x3E)) = 'T';

		// Partition 2 - Kernel
		*((uint64_t *) (0x100000 + 0x80 + 0x00)) = 0x7553734949464555ULL; // Type GUID
		*((uint64_t *) (0x100000 + 0x80 + 0x08)) = 0x2164726148726570ULL;
		__asm__ __volatile__("rdrand %0": "=a" (r)); // Partition GUID
		*((uint32_t *) (0x100000 + 0x80 + 0x10)) = r;
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x80 + 0x14)) = r;
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x80 + 0x18)) = r;
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x80 + 0x1C)) = r;
		*((uint64_t *) (0x100000 + 0x80 + 0x20)) = 0x400;
		*((uint64_t *) (0x100000 + 0x80 + 0x28)) = 0x3FFF;
		*((uint8_t *) (0x100000 + 0x80 + 0x38)) = 'K';
		*((uint8_t *) (0x100000 + 0x80 + 0x3A)) = 'e';
		*((uint8_t *) (0x100000 + 0x80 + 0x3C)) = 'r';
		*((uint8_t *) (0x100000 + 0x80 + 0x3E)) = 'n';
		*((uint8_t *) (0x100000 + 0x80 + 0x40)) = 'e';
		*((uint8_t *) (0x100000 + 0x80 + 0x42)) = 'l';

		// Partition 3 - APFS
		*((uint64_t *) (0x100000 + 0x100 + 0x00)) = 0x11AA00007C3457EFULL; // Type GUID
		*((uint64_t *) (0x100000 + 0x100 + 0x08)) = 0xACEC4365300011AAULL;
		__asm__ __volatile__("rdrand %0": "=a" (r)); // Partition GUID
		*((uint32_t *) (0x100000 + 0x100 + 0x10)) = r;
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x100 + 0x14)) = r;
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x100 + 0x18)) = r;
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x100 + 0x1C)) = r;
		*((uint64_t *) (0x100000 + 0x100 + 0x20)) = 0x4000;
		*((uint64_t *) (0x100000 + 0x100 + 0x28)) = *((uint64_t *) 0x70C8) - 0x800;
		*((uint8_t *) (0x100000 + 0x100 + 0x38)) = 'A';
		*((uint8_t *) (0x100000 + 0x100 + 0x3A)) = 'P';
		*((uint8_t *) (0x100000 + 0x100 + 0x3C)) = 'F';
		*((uint8_t *) (0x100000 + 0x100 + 0x3E)) = 'S';

		// Partition 3 - SWAP
		*((uint64_t *) (0x100000 + 0x180 + 0x00)) = 0x4157536F54776F48ULL; // Type GUID
		*((uint64_t *) (0x100000 + 0x180 + 0x08)) = 0x3F79726F6D654D50ULL;
		__asm__ __volatile__("rdrand %0": "=a" (r)); // Partition GUID
		*((uint32_t *) (0x100000 + 0x180 + 0x10)) = r;
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x180 + 0x14)) = r;
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x180 + 0x18)) = r;
		__asm__ __volatile__("rdrand %0": "=a" (r));
		*((uint32_t *) (0x100000 + 0x180 + 0x1C)) = r;
		*((uint64_t *) (0x100000 + 0x180 + 0x20)) = *((uint64_t *) 0x70C8) - 0x800;
		*((uint64_t *) (0x100000 + 0x180 + 0x28)) = *((uint64_t *) 0x70C8) - 34;
		*((uint8_t *) (0x100000 + 0x180 + 0x38)) = 'S';
		*((uint8_t *) (0x100000 + 0x180 + 0x3A)) = 'W';
		*((uint8_t *) (0x100000 + 0x180 + 0x3C)) = 'A';
		*((uint8_t *) (0x100000 + 0x180 + 0x3E)) = 'P';

		write_disk(2, 1, (uint16_t *) 0x100000);
		write_disk(*((uint64_t *) 0x70C8) - 34, 1, (uint16_t *) 0x100000);

		puts("Partitions burned\n");

		// Partition Table Header
		// MBR included in bootsect.bin
		*((uint32_t *) (0x100000 + 0x58)) = CRC32((const uint8_t *) 0x100000, 0x200); // CRC checksum

		*((uint64_t *) (0x100000 + 0x00)) = 0x5452415020494645ULL;  // 0x00 "EFI PART"
		*((uint32_t *) (0x100000 + 0x08)) = 0x00010000U; // Revision 1.0
		*((uint32_t *) (0x100000 + 0x0C)) = 0x0000005CU; // Header size
		*((uint32_t *) (0x100000 + 0x10)) = 0x00000000U; // CRC - calculate last, put 0
		*((uint32_t *) (0x100000 + 0x14)) = 0x00000000U; // Reserved
		*((uint64_t *) (0x100000 + 0x18)) = 0x0000000000000000001ULL;  // Current LBA
		*((uint64_t *) (0x100000 + 0x20)) = 0x0000000000000000020ULL;  // Backup  LBA :P no backup atm
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

		*((uint32_t *) (0x100000 + 0x10)) = CRC32((const uint8_t *) 0x100000, 0x5b); // CRC - calculate last, put 0

		/* Filling the rest of the partition with zeros. */
		for (uint_fast16_t i = 0; i < 420; i++)
			*((uint8_t *) (0x100000 + 0x5C + i)) = 0x0;

		write_disk(1, 1, (uint16_t *) 0x100000);
		write_disk(*((uint64_t *) 0x70C8) - 1, 1, (uint16_t *) 0x100000);

		puts("Partition table header burned");

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

				write_disk(0, 1, (uint16_t *) 0x100800);
			}

			// If the file is second.bin
			if (HEDLEY_UNLIKELY(strcmp("second.bin", (const char *) (addr + 33)) != 0)) {
				read_cdrom(*((uint32_t *) (addr + 2)), *((uint32_t *) (addr + 10)) / 2048 + 1, (uint16_t *) 0x100800);

				puts("second.bin read\n");

				write_disk(0x20, *((uint32_t *) (addr + 10)) / 512 + 1, (uint16_t *) 0x100800);

				puts("second.bin burned\n");
			}

			// If the file is kernel.bin
			if (HEDLEY_UNLIKELY(strcmp("kernel.bin", (const char *) (addr + 33)) != 0)) {
				read_cdrom(*((uint32_t *) (addr + 2)), *((uint32_t *) (addr + 10)) / 2048 + 1, (uint16_t *) 0x100800);

				puts("kernel.bin read\n");

				write_disk(0x400, *((uint32_t *) (addr + 10)) / 512 + 1, (uint16_t *) 0x100800);

				puts("kernel.bin burned\n");
			}

			addr += *((uint8_t *) addr);
		}
	}

	eject_cdrom();

	uint8_t good = 0x02;
	while (good & 0x02)
		good = inb(0x64);
	outb(0x64, 0xFE);

	__asm__ __volatile__("sti");
	__asm__ __volatile__("int $0");
}

const uint32_t CRCTable[256] = {
		0, 0x77073096, 0xEE0E612C, 0x990951BA,
		0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
		0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
		0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
		0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
		0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
		0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
		0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
		0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
		0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
		0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
		0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
		0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
		0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
		0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
		0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
		0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
		0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
		0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
		0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
		0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
		0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
		0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
		0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
		0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
		0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
		0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
		0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
		0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
		0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
		0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
		0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
		0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
		0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
		0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
		0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
		0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
		0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
		0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
		0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
		0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
		0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
		0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
		0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
		0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
		0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
		0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
		0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
		0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
		0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
		0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
		0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
		0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
		0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
		0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
		0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
		0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
		0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
		0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
		0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
		0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
		0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
		0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
		0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

uint32_t CRC32(const uint8_t data[], size_t data_length) {
	uint32_t crc32 = 0xFFFFFFFF;

	for (size_t i = 0; i < data_length; i++) {
		crc32 = (crc32 >> 8) ^ CRCTable[(crc32 ^ data[i]) & 0xff];  // CRCTable is an array of 256 32-bit constants
	}

	return crc32 ^ 0xFFFFFFFF;
}

static void ata_io_wait(const uint8_t p) {
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
	inb(p + ALTERNATE_STATUS + CONTROL);
}

bool identify_disk(void) {
	reset_ata(drive_port);

	outb(drive_port + DRIVE_SELECT, 0xA0 | (drive_slave << 4)); /* Drive select */

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

void write_disk(const uint64_t lba, const uint16_t sectors, uint16_t *buffer) {
	outb(drive_port + DRIVE_SELECT, 0x40 | (drive_slave << 4)); // drive select with lba bit set
	ata_io_wait(drive_port);
	outb(drive_port + SECTOR_COUNT, sectors >> 8);
	outb(drive_port + LBA_LOW, (lba & 0x0000ff000000) >> 24);
	outb(drive_port + LBA_MID, (lba & 0x00ff00000000) >> 32);
	outb(drive_port + LBA_HIGH, (lba & 0xff0000000000) >> 40);

	outb(drive_port + SECTOR_COUNT, sectors);
	outb(drive_port + LBA_LOW, (lba & 0x000000ff) >> 0);
	outb(drive_port + LBA_MID, (lba & 0x0000ff00) >> 8);
	outb(drive_port + LBA_HIGH, (lba & 0x00ff0000) >> 16);
	outb(drive_port + COMMAND_REGISTER, 0x34);  // Write disk command

	for (uint32_t i = 0; i < sectors; i++) {
		ata_io_wait(drive_port);

		while (1) {
			uint8_t status = inb(drive_port + COMMAND_REGISTER);

			if (!(status & 0x80) && (status & 0x08))
				break;
			else if (status & 0x01)
				return;

			ata_io_wait(drive_port);
		}

		for (int j = 0; j < 256; j++, buffer++)
			outw(drive_port + DATA, *buffer);
	}

	// Flush cache
	ata_io_wait(drive_port);
	outb(drive_port + DRIVE_SELECT, 0xA0);
	ata_io_wait(drive_port);
	outb(drive_port + COMMAND_REGISTER, 0xE7);
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
