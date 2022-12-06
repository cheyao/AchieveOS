//
// Created by cheyao on 06.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <kernel/screen.h>
#include <stdint.h>


void circle(int32_t xc, int32_t yc, int32_t r) {
	int32_t x = 0, y = r;
	int32_t d = 3 - 2 * r;

	putPixel(xc + x, yc + y, rgb(0xFF, 0xFF, 0xFF));
	putPixel(xc + x, yc - y, rgb(0xFF, 0xFF, 0xFF));
	putPixel(xc - x, yc + y, rgb(0xFF, 0xFF, 0xFF));
	putPixel(xc - x, yc - y, rgb(0xFF, 0xFF, 0xFF));
	putPixel(xc + y, yc + x, rgb(0xFF, 0xFF, 0xFF));
	putPixel(xc + y, yc - x, rgb(0xFF, 0xFF, 0xFF));
	putPixel(xc - y, yc + x, rgb(0xFF, 0xFF, 0xFF));
	putPixel(xc - y, yc - x, rgb(0xFF, 0xFF, 0xFF));

	while (y >= x) {
		x++;

		if (d > 0) {
			y--;
			d = d + 4 * (x - y) + 10;
		} else {
			d = d + 4 * x + 6;
		}

		putPixel(xc + x, yc + y, rgb(0xFF, 0xFF, 0xFF));
		putPixel(xc + x, yc - y, rgb(0xFF, 0xFF, 0xFF));
		putPixel(xc - x, yc + y, rgb(0xFF, 0xFF, 0xFF));
		putPixel(xc - x, yc - y, rgb(0xFF, 0xFF, 0xFF));
		putPixel(xc + y, yc + x, rgb(0xFF, 0xFF, 0xFF));
		putPixel(xc + y, yc - x, rgb(0xFF, 0xFF, 0xFF));
		putPixel(xc - y, yc + x, rgb(0xFF, 0xFF, 0xFF));
		putPixel(xc - y, yc - x, rgb(0xFF, 0xFF, 0xFF));
	}
}
