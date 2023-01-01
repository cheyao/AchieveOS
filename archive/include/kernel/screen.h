//
// Created by cheyao on 18.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _SCREEN_H
#define _SCREEN_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

#define BUFFER 0x200000
#define DOUBLE_BUFFER 0x400000
#define WIDTH 1024
#define HEIGHT 768
#define rgb(r, g, b) (((uint16_t)(((r)&0x1F) << 11) + (((g)&0x3F) << 5) + ((b)&0x1F)))

static inline void put_pixel(uint32_t _x, uint32_t _y, uint16_t __color) {
	*((uint16_t *) DOUBLE_BUFFER + _x + _y * WIDTH) = __color;
}

static inline void swap_buffers(void) {
	memcpy(BUFFER, DOUBLE_BUFFER, WIDTH * HEIGHT * 2);
}

typedef struct {
	uint32_t x, y;
} __attribute__((packed)) Vector2;

void circle(Vector2 P0, int32_t radius, uint16_t color);

void line(Vector2 P0, Vector2 P1, uint16_t color);

typedef uint16_t color_t;

void cubic_bezier_curve(const Vector2 p[4], color_t color);

void quadratic_bezier_curve(const Vector2 p[3], color_t color);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  //_SCREEN_H
