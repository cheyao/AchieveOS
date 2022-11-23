#include <kernel/cd.h>
#include <kernel/idt.h>
#include <kernel/keyboard.h>
#include <stdio.h>

Disk disks[4] = {
		{.port = BUS_PRIMARY, .type = UNKNOWN, .drive_select_command = 0xA0, .removable = false, .protocol = OTHER, .control = 0x3f6},
		{.port = BUS_PRIMARY, .type = UNKNOWN, .drive_select_command = 0xB0, .removable = false, .protocol = OTHER, .control = 0x3f6},
		{.port = BUS_SECONDARY, .type = UNKNOWN, .drive_select_command = 0xA0, .removable = false, .protocol = OTHER, .control = 0x370},
		{.port = BUS_SECONDARY, .type = UNKNOWN, .drive_select_command = 0xB0, .removable = false, .protocol = OTHER, .control = 0x370},
};
uint8_t cdrom_port = 5;

void main(void) {
	update_cursor(0);
	init_idt();

	atapi(&disks[0]);
	atapi(&disks[1]);
	atapi(&disks[2]);
	atapi(&disks[3]);

	for (int i = 0; i < 4; i++) {
		if (disks[i].type == CDROM) {
			cdrom_port = i;
		}
	}

	printf("Reading disk %d\n", cdrom_port);
	read_cdrom(&disks[cdrom_port], 0x10, 1);

	printf("Total of %dMb", *((int32_t *) 0x100050) * 2);  // 901
}
