//
// Created by cheyao on 06.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <kernel/screen.h>
#include <stdint.h>

void circle(Vector2 Pc, int32_t r, uint16_t color) {
	int32_t x = 0, y = r;
	int32_t d = 3 - 2 * r;

	putPixel(Pc.x + x, Pc.y + y, color);
	putPixel(Pc.x + x, Pc.y - y, color);
	putPixel(Pc.x - x, Pc.y + y, color);
	putPixel(Pc.x - x, Pc.y - y, color);
	putPixel(Pc.x + y, Pc.y + x, color);
	putPixel(Pc.x + y, Pc.y - x, color);
	putPixel(Pc.x - y, Pc.y + x, color);
	putPixel(Pc.x - y, Pc.y - x, color);

	while (y >= x) {
		x++;

		if (d > 0) {
			y--;
			d = d + 4 * (x - y) + 10;
		} else {
			d = d + 4 * x + 6;
		}

		putPixel(Pc.x + x, Pc.y + y, color);
		putPixel(Pc.x + x, Pc.y - y, color);
		putPixel(Pc.x - x, Pc.y + y, color);
		putPixel(Pc.x - x, Pc.y - y, color);
		putPixel(Pc.x + y, Pc.y + x, color);
		putPixel(Pc.x + y, Pc.y - x, color);
		putPixel(Pc.x - y, Pc.y + x, color);
		putPixel(Pc.x - y, Pc.y - x, color);
	}
}

static inline void plotLineLow(Vector2 P0, Vector2 P1, const uint16_t color) {
	int32_t dx = (int32_t) (P1.x - P0.x);
	int32_t dy = (int32_t) (P1.y - P0.y);
	uint32_t yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	uint32_t D = (2 * dy) - dx;
	uint32_t y = P0.y;

	for (uint32_t x = P0.x; x < P1.x; x++) {
		putPixel(x, y, color);
		if (D > 0) {
			y = y + yi;
			D = D + (2 * (dy - dx));
		} else {
			D = D + 2 * dy;
		}
	}
}

static void plotLineHigh(Vector2 P0, Vector2 P1, const uint16_t color) {
	int32_t dx = (int32_t) (P1.x - P0.x);
	int32_t dy = (int32_t) (P1.y - P0.y);
	uint32_t xi = 1;
	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}
	uint32_t D = (2 * dx) - dy;
	uint32_t x = P0.x;

	for (uint32_t y = P0.y; y < P1.y; y++) {
		putPixel(x, y, color);
		if (D > 0) {
			x = x + xi;
			D = D + (2 * (dx - dy));
		} else {
			D = D + 2 * dx;
		}
	}
}

void line(Vector2 P0, Vector2 P1, const uint16_t color) {
	if (P1.y - P0.y < P1.x - P0.x) {
		if (P0.x > P1.x) {
			plotLineLow(P1, P0, color);
		} else {
			plotLineLow(P0, P1, color);
		}
	} else {
		if (P0.y > P1.y) {
			plotLineHigh(P1, P0, color);
		} else {
			plotLineHigh(P0, P1, color);
		}
	}
}
