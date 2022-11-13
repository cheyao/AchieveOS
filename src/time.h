//
// Created by cheyao on 12.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _TIME_H
#define _TIME_H

#include "types.h"

extern u64int ticks;

u32int sleep(u64int seconds);

void tick(void);

int oct2bin(unsigned char *str, int size);

typedef u64int time_t;

#endif //_TIME_H
