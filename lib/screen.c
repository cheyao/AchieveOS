//
// Created by cheyao on 06.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <kernel/screen.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

void circle(Vector2 Pc, int32_t r, const uint16_t color) {
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

void line(Vector2 p0, Vector2 p1, const uint16_t color) {
	bool yLonger = false;
	int32_t shortLen = (int32_t) (p1.y - p0.y);
	int32_t longLen = (int32_t) (p1.x - p0.x);
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
			longLen += (int32_t) p0.y;
			for (int j = (int32_t) (0x8000 + (p0.x << 16)); (int32_t) p0.y <= longLen; ++p0.y) {
				putPixel(j >> 16, p0.y, color);
				j += decInc;
			}
			return;
		}
		longLen += (int32_t) p0.y;
		for (int j = (int32_t) (0x8000 + (p0.x << 16)); (int32_t) p0.y >= longLen; --p0.y) {
			putPixel(j >> 16, p0.y, color);
			j -= decInc;
		}
		return;
	}

	if (longLen > 0) {
		longLen += (int32_t) p0.x;
		for (int j = (int32_t) (0x8000 + (p0.y << 16)); (int32_t) p0.x <= longLen; ++p0.x) {
			putPixel(p0.x, j >> 16, color);
			j += decInc;
		}
		return;
	}
	longLen += (int32_t) p0.x;
	for (int j = (int32_t) (0x8000 + (p0.y << 16)); (int32_t) p0.x >= longLen; --p0.x) {
		putPixel(p0.x, j >> 16, color);
		j -= decInc;
	}
}

static Vector2 midpoint(Vector2 p1, Vector2 p2) {
	return (Vector2) {.x = (int) (p2.x - p1.x) / 2 + p1.x, .y = (int) (p2.y - p1.y) / 2 + p1.y};
}

static void split_curve(const Vector2 p[4], Vector2 out[2][4]) {
	const Vector2 p1 = p[0];
	const Vector2 p2 = p[1];
	const Vector2 p3 = p[2];
	const Vector2 p4 = p[3];
	const Vector2 p12 = midpoint(p1, p2);
	const Vector2 p23 = midpoint(p2, p3);
	const Vector2 p34 = midpoint(p4, p3);
	const Vector2 p123 = midpoint(p12, p23);
	const Vector2 p234 = midpoint(p23, p34);
	const Vector2 p1234 = {.x = (int) (p234.x - p123.x) / 2 + p123.x,
			.y = (int) (p234.y - p123.y) / 2 + p123.y};
	out[0][0] = p1;
	out[0][1] = p12;
	out[0][2] = p123;
	out[0][3] = p1234;
	out[1][0] = p1234;
	out[1][1] = p234;
	out[1][2] = p34;
	out[1][3] = p4;
}

static uint32_t flatness(const Vector2 p[4]) {
	const Vector2 p1 = p[0];
	const Vector2 cp1 = p[1];
	const Vector2 cp2 = p[2];
	const Vector2 p2 = p[3];
	uint32_t ux = (3 * cp1.x - 2 * p1.x - p2.x) * (3 * cp1.x - 2 * p1.x - p2.x);
	uint32_t uy = (3 * cp1.y - 2 * p1.y - p2.y) * (3 * cp1.y - 2 * p1.y - p2.y);
	uint32_t vx = (3 * cp2.x - 2 * p2.x - p1.x) * (3 * cp2.x - 2 * p2.x - p1.x);
	uint32_t vy = (3 * cp2.y - 2 * p2.y - p1.y) * (3 * cp2.y - 2 * p2.y - p1.y);
	if (ux < vx) ux = vx;
	if (uy < vy) uy = vy;
	return ux + uy;
}

void cubic_bezier_curve(const Vector2 p[4], const color_t color) {
#define THRESHOLD 10
	if (flatness(p) < THRESHOLD) {
		line(p[0], p[3], color);
		return;
	} else {
		Vector2 split[2][4] = {{[3] = {0, 0}},
		                       {[3] = {0, 0}}};
		split_curve(p, split);
		cubic_bezier_curve(split[0], color);
		cubic_bezier_curve(split[1], color);
	}
}

static void quadratic_split_curve(const Vector2 p[3], Vector2 out[2][3]) {
	const Vector2 p12 = midpoint(p[0], p[1]);
	const Vector2 p23 = midpoint(p[1], p[2]);
	const Vector2 p123 = midpoint(p12, p23);
	out[0][0] = p[0];
	out[0][1] = p12;
	out[0][2] = p123;
	out[1][0] = p123;
	out[1][1] = p23;
	out[1][2] = p[2];
}

static int32_t quadratic_flatness(const Vector2 p[3]) {
	return ((int32_t) p[1].x - (((int32_t) p[0].x + (int32_t) p[2].x) / 2) +
	        (int32_t) p[1].y - (((int32_t) p[0].y + (int32_t) p[2].y) / 2));
}

void quadratic_bezier_curve(const Vector2 p[3], const color_t color) {
	if (quadratic_flatness(p) < 2) {
		line(p[0], p[2], color);
		return;
	} else {
		Vector2 split[2][3] = {{[2] = {0, 0}},
		                       {[2] = {0, 0}}};
		quadratic_split_curve(p, split);
		quadratic_bezier_curve(split[0], color);
		quadratic_bezier_curve(split[1], color);
	}
}
