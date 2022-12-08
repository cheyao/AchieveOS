//
// Created by cheyao on 06.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <kernel/screen.h>
#include <math.h>
#include <stdbool.h>
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

void line(Vector2 p0, Vector2 p1, uint16_t color) {
	bool yLonger = false;
	int shortLen = p1.y - p0.y;
	int longLen = p1.x - p0.x;
	if (abs(shortLen) > abs(longLen)) {
		int swap = shortLen;
		shortLen = longLen;
		longLen = swap;
		yLonger = true;
	}
	int decInc;
	if (longLen == 0)
		decInc = 0;
	else
		decInc = (shortLen << 16) / longLen;

	if (yLonger) {
		if (longLen > 0) {
			longLen += p0.y;
			for (int j = 0x8000 + (p0.x << 16); p0.y <= longLen; ++p0.y) {
				putPixel(j >> 16, p0.y, color);
				j += decInc;
			}
			return;
		}
		longLen += p0.y;
		for (int j = 0x8000 + (p0.x << 16); p0.y >= longLen; --p0.y) {
			putPixel(j >> 16, p0.y, color);
			j -= decInc;
		}
		return;
	}

	if (longLen > 0) {
		longLen += p0.x;
		for (int j = 0x8000 + (p0.y << 16); p0.x <= longLen; ++p0.x) {
			putPixel(p0.x, j >> 16, color);
			j += decInc;
		}
		return;
	}
	longLen += p0.x;
	for (int j = 0x8000 + (p0.y << 16); p0.x >= longLen; --p0.x) {
		putPixel(p0.x, j >> 16, color);
		j -= decInc;
	}
}
