#include <kernel/ata.h>     // for OTHER, UNKNOWN, BUS_PRIMARY, BUS_SECONDARY
#include <kernel/screen.h>  // for Vector2, rgb, circle, line, quadratic_bez...
#include <kernel/idt.h>  // for Vector2, rgb, circle, line, quadratic_bez...
#include <stdbool.h>        // for false
#include <stdint.h>         // for uint16_t, uint8_t
#include <string.h>         // for memset

Disk disks[4] = {
		{.port = BUS_PRIMARY, .type = UNKNOWN, .drive_select_command = 0xA0, .removable = false, .protocol = OTHER, .control = 0x3f6},
		{.port = BUS_PRIMARY, .type = UNKNOWN, .drive_select_command = 0xB0, .removable = false, .protocol = OTHER, .control = 0x3f6},
		{.port = BUS_SECONDARY, .type = UNKNOWN, .drive_select_command = 0xA0, .removable = false, .protocol = OTHER, .control = 0x370},
		{.port = BUS_SECONDARY, .type = UNKNOWN, .drive_select_command = 0xB0, .removable = false, .protocol = OTHER, .control = 0x370},
};

uint8_t cdrom_port = 5;

void main(void) {
	init_idt();

	circle((Vector2) {WIDTH / 2, HEIGHT / 2}, 300, rgb(0xFF, 0xFF, 0xFF));

	quadratic_bezier_curve((Vector2[4]) {{.x = 50, .y = 50},
	                                     {.x = 75, .y = 100},
	                                     {.x = 100, .y = 50}},
	                       rgb(0xFF, 0xFF, 0xFF));

	memset((uint16_t *) (BUFFER + WIDTH * HEIGHT / 2), rgb(0xFF, 0, 0xFF), WIDTH * HEIGHT / 2);

	line((Vector2) {567, 333}, (Vector2) {630, 604}, rgb(0x4d, 0x25, 0x8e));

	memcpy((uint16_t *) (BUFFER + WIDTH * HEIGHT / 2 - 1000), (uint16_t *) (BUFFER), WIDTH * HEIGHT / 2);

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
