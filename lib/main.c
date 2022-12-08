#include <kernel/cd.h>
#include <kernel/idt.h>
#include <kernel/screen.h>
#include <math.h>
#include <stdint.h>

Disk disks[4] = {
		{.port = BUS_PRIMARY, .type = UNKNOWN, .drive_select_command = 0xA0, .removable = false, .protocol = OTHER, .control = 0x3f6},
		{.port = BUS_PRIMARY, .type = UNKNOWN, .drive_select_command = 0xB0, .removable = false, .protocol = OTHER, .control = 0x3f6},
		{.port = BUS_SECONDARY, .type = UNKNOWN, .drive_select_command = 0xA0, .removable = false, .protocol = OTHER, .control = 0x370},
		{.port = BUS_SECONDARY, .type = UNKNOWN, .drive_select_command = 0xB0, .removable = false, .protocol = OTHER, .control = 0x370},
};

uint8_t cdrom_port = 5;

Vector2 midpoint(Vector2 p1, Vector2 p2) {
	return (Vector2) {.x = (p2.x - p1.x) / 2 + p1.x, .y = (p2.y - p1.y) / 2 + p1.y};
}

void splitCurve(Vector2 p[4], Vector2 (*out)[2][4]) {
	Vector2 p1 = p[0];
	Vector2 p2 = p[1];
	Vector2 p3 = p[2];
	Vector2 p4 = p[3];
	Vector2 p12 = midpoint(p1, p2);
	Vector2 p23 = midpoint(p2, p3);
	Vector2 p34 = midpoint(p4, p3);
	Vector2 p123 = midpoint(p12, p23);
	Vector2 p234 = midpoint(p23, p34);
	Vector2 p1234 = {.x = (p234.x - p123.x) / 2 + p123.x,
			.y = (p234.y - p123.y) / 2 + p123.y};
	*out[0][0] = p1;
	*out[0][1] = p12;
	*out[0][2] = p123;
	*out[0][3] = p1234;
	*out[1][0] = p1234;
	*out[1][1] = p234;
	*out[1][2] = p34;
	*out[1][3] = p4;
}

uint32_t flatness(Vector2 curve[4]) {
	Vector2 p1 = curve[0];
	Vector2 cp1 = curve[1];
	Vector2 cp2 = curve[2];
	Vector2 p2 = curve[3];
	uint32_t ux = (3 * cp1.x - 2 * p1.x - p2.x) << 1;
	uint32_t uy = (3 * cp1.y - 2 * p1.y - p2.y) << 1;
	uint32_t vx = (3 * cp2.x - 2 * p2.x - p1.x) << 1;
	uint32_t vy = (3 * cp2.y - 2 * p2.y - p1.y) << 1;
	if (ux < vx) ux = vx;
	if (uy < vy) uy = vy;
	return ux + uy;
}

void curve(Vector2 P[4], uint16_t color) {
	if (flatness(P) < 100) {
		return line(P[0], P[3], color);
	} else {
		Vector2 split[2][4];
		splitCurve(P, &split);
		curve(split[0], color);
		curve(split[1], color);
	}
}

void main(void) {
	init_idt();

	circle((Vector2) {WIDTH / 2, HEIGHT / 2}, 300, rgb(0xFF, 0xFF, 0xFF));
	circle((Vector2) {WIDTH / 2 - 100, HEIGHT / 3}, 50, rgb(0xFF, 0xFF, 0xFF));
	circle((Vector2) {WIDTH / 2 + 100, HEIGHT / 3}, 50, rgb(0xFF, 0xFF, 0xFF));

	line((Vector2) {50, 60}, (Vector2) {580, 190}, rgb(0xFF, 0xFF, 0xFF));

	curve((Vector2[]) {{0,   0},
	                   {0,   100},
	                   {100, 100},
	                   {100, 0}}, rgb(0xFF, 0xFF, 0xFF));

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
