#include <kernel/ata.h>     // for OTHER, UNKNOWN, BUS_PRIMARY, BUS_SECONDARY
#include <kernel/screen.h>  // for Vector2, rgb, circle, line, quadratic_bez...
#include <kernel/idt.h>  // for Vector2, rgb, circle, line, quadratic_bez...
#include <stdbool.h>        // for false
#include <stdint.h>         // for uint16_t, uint8_t
#include <string.h>         // for memset
#include <stdio.h>         // for memset

Disk disks[4] = {
		{.port = BUS_PRIMARY, .type = UNKNOWN, .drive_select_command = 0xA0, .removable = false, .protocol = OTHER, .control = 0x3f6},
		{.port = BUS_PRIMARY, .type = UNKNOWN, .drive_select_command = 0xB0, .removable = false, .protocol = OTHER, .control = 0x3f6},
		{.port = BUS_SECONDARY, .type = UNKNOWN, .drive_select_command = 0xA0, .removable = false, .protocol = OTHER, .control = 0x370},
		{.port = BUS_SECONDARY, .type = UNKNOWN, .drive_select_command = 0xB0, .removable = false, .protocol = OTHER, .control = 0x370},
};

uint8_t cdrom_port = 5;

int snake(void);

void main(void) {
	init_idt();

	printf("Hello %s%c", "world", '!');

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
