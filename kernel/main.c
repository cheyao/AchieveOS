#include <kernel/ata.h>  // for OTHER, UNKNOWN, BUS_PRIMARY, BUS_SECONDARY
#include <kernel/idt.h>  // for Vector2, rgb, circle, line, quadratic_bez...
#include <stdbool.h>     // for false
#include <stdint.h>      // for uint16_t, uint8_t
#include <stdio.h>       // for memset
#include <kernel/memory.h>

Disk disks[4] = {
		{.port = BUS_PRIMARY, .type = UNKNOWN, .drive_select_command = 0xA0, .removable = false, .protocol = OTHER, .control = 0x3f6},
		{.port = BUS_PRIMARY, .type = UNKNOWN, .drive_select_command = 0xB0, .removable = false, .protocol = OTHER, .control = 0x3f6},
		{.port = BUS_SECONDARY, .type = UNKNOWN, .drive_select_command = 0xA0, .removable = false, .protocol = OTHER, .control = 0x370},
		{.port = BUS_SECONDARY, .type = UNKNOWN, .drive_select_command = 0xB0, .removable = false, .protocol = OTHER, .control = 0x370},
};

typedef struct SMAP_entry {
	uint64_t Base;
	uint64_t Length;
	uint32_t Type;  // entry Type
	uint32_t ACPI;  // extended
} __attribute__((packed)) SMAP_entry_t;

uint8_t cdrom_port = 5;

static const char *restrict types[] = {
		"Error! (0)",
		"Available memory (1)",
		"Reserved memory (2)",
		"ACPI reclaim memory (3)",
		"ACPI NVS memory (4)",
};

void main(void) {
	__init_idt();
	__init_memory();

	SMAP_entry_t *entry = (SMAP_entry_t *) 0x2004;

	puts("| Base             |Length            |Type                   |");
	puts("|------------------|------------------|-----------------------|");
	for (int i = 0; i < *((uint16_t *) 0x2000) - 1; i++) {
		printf("|%#018lx|%#018lx|%-23s|\n", entry->Base, entry->Length, types[entry->Type > 4 ? 2 : entry->Type]);
		entry++;
	}

	// printf("One %d\n", 1);
	// identify(&disks[0]);
	// printf("2\n");
	// identify(&disks[1]);
	// printf("3\bh\n");
	// identify(&disks[2]);
	// printf("Type: %d\n", disks[0].type);
	// puts("Type: 4\n");
	// identify(&disks[3]);
	//
	// for (int i = 0; i < 4; i++) {
	//	if (disks[i].type == CDROM) {
	//		cdrom_port = i;
	//	}
	//}
	//
	// read_cdrom(&disks[cdrom_port], 0x10, 1, (uint16_t *)0x100000);
	// read_cdrom(&disks[cdrom_port], *((int32_t *)0x10009e), 1, (uint16_t *)0x100000);

	// puts("Welcome to Achieve OS\nUsage: Press [1] for ls, Press [2] for cd\n");
}
