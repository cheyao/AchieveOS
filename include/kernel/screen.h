//
// Created by cheyao on 18.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _SCREEN_H
#define _SCREEN_H


#define BUFFER 0x200000
#define WIDTH 1024
#define HEIGHT 768
#define rgb(r, g, b) (((uint16_t)(((r)&0b11111) << 11) + (((g)&0b111111) << 5) + ((b)&0b11111)))

#endif  //_SCREEN_H
