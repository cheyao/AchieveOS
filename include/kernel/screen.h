//
// Created by cheyao on 18.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _SCREEN_H
#define _SCREEN_H

#include <stdint.h>

#define BUFFER 0x200000
#define WIDTH 1024
#define HEIGHT 768
#define rgb(r, g, b) (((uint16_t)(((r)&0b11111) << 11) + (((g)&0b111111) << 5) + ((b)&0b11111)))

static __inline void putPixel(uint32_t __x, uint32_t __y, uint16_t __color) {
	*((uint16_t *) BUFFER + __x + __y * WIDTH) = __color;
}

void circle(int32_t x, int32_t y, int32_t radius);

#endif  //_SCREEN_H
