#include <kernel/cd.h>
#include <kernel/idt.h>
#include <string.h>
#include <stdint.h>
#include "kernel/screen.h"

Disk disks[4] = {
		{.port = BUS_PRIMARY, .type = UNKNOWN, .drive_select_command = 0xA0, .removable = false, .protocol = OTHER, .control = 0x3f6},
		{.port = BUS_PRIMARY, .type = UNKNOWN, .drive_select_command = 0xB0, .removable = false, .protocol = OTHER, .control = 0x3f6},
		{.port = BUS_SECONDARY, .type = UNKNOWN, .drive_select_command = 0xA0, .removable = false, .protocol = OTHER, .control = 0x370},
		{.port = BUS_SECONDARY, .type = UNKNOWN, .drive_select_command = 0xB0, .removable = false, .protocol = OTHER, .control = 0x370},
};

uint8_t cdrom_port = 5;

const uint8_t gylphs[][8] = {
		{0xc6, 0xc6, 0xc6, 0xfe, 0xc6, 0xc6, 0xc6, 0x00},
		{0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x00}
};

void main(void) {
	init_idt();

	circle(WIDTH / 2, HEIGHT / 2, 300);
	circle(WIDTH / 2 - 100, HEIGHT / 3, 50);
	circle(WIDTH / 2 + 100, HEIGHT / 3, 50);


	// puts("One\n");
	// identify(&disks[0]);
	// puts("2\n");
	// identify(&disks[1]);
	// puts("3\n");
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
