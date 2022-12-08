#include <kernel/cd.h>
#include <kernel/idt.h>
#include <kernel/screen.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>

Disk disks[4] = {
		{.port = BUS_PRIMARY, .type = UNKNOWN, .drive_select_command = 0xA0, .removable = false, .protocol = OTHER, .control = 0x3f6},
		{.port = BUS_PRIMARY, .type = UNKNOWN, .drive_select_command = 0xB0, .removable = false, .protocol = OTHER, .control = 0x3f6},
		{.port = BUS_SECONDARY, .type = UNKNOWN, .drive_select_command = 0xA0, .removable = false, .protocol = OTHER, .control = 0x370},
		{.port = BUS_SECONDARY, .type = UNKNOWN, .drive_select_command = 0xB0, .removable = false, .protocol = OTHER, .control = 0x370},
};

uint8_t cdrom_port = 5;

#define perpendicular(point) ((Vector2){.x = -(point).y, .y = (point).x})

Vector2 calcBezierAtT(Vector2 p[4], double t) {
	uint32_t x = (uint32_t) ((1 - t) * (1 - t) * (1 - t) * p[0].x + 3 * (1 - t) * (1 - t) * t * p[1].x +
	                         3 * (1 - t) * t * t * p[2].x + t * t * t * p[3].x);
	uint32_t y = (uint32_t) ((1 - t) * (1 - t) * (1 - t) * p[0].y + 3 * (1 - t) * (1 - t) * t * p[1].y +
	                         3 * (1 - t) * t * t * p[2].y + t * t * t * p[3].y);
	return (Vector2) {x, y};
}

void curve(Vector2 points[4], uint16_t color) {
	for (double i = 0; i <= 1; i += 0.0001) {
		Vector2 v = calcBezierAtT(points, i);

		putPixel(v.x, v.y, color);
	}
}

void main(void) {
	// init_idt();

	circle((Vector2) {WIDTH / 2, HEIGHT / 2}, 300, rgb(0xFF, 0xFF, 0xFF));

	curve((Vector2[4]) {{.x = WIDTH / 2 - 300, .y = HEIGHT / 2},
	                    {.x = WIDTH / 5 * 2, .y = 50},
	                    {.x = WIDTH / 5 * 3, .y = HEIGHT - 50},
	                    {.x = WIDTH / 2 + 300, .y = HEIGHT / 2}},
	      rgb(0xFF, 0xFF, 0xFF));

	line((Vector2) {567, 333}, (Vector2) {630, 604}, rgb(0x4d, 0x25, 0x8e));
	line((Vector2) {888, 539}, (Vector2) {93, 48}, rgb(0xb4, 0xfa, 0x7e));
	line((Vector2) {441, 408}, (Vector2) {731, 229}, rgb(0xf6, 0xce, 0x6e));
	line((Vector2) {137, 221}, (Vector2) {529, 612}, rgb(0x96, 0xe2, 0xde));

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
