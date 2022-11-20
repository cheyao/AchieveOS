//
// Created by cheyao on 12.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _TIME_H
#define _TIME_H

#include <stdint.h>

extern uint64_t ticks;

uint64_t sleep(uint64_t seconds);

uint64_t sleepms(uint64_t milliseconds);

void tick(void);

typedef uint64_t time_t;

#endif  //_TIME_H
