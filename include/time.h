//
// Created by cheyao on 12.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _TIME_H
#define _TIME_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <kernel/idt.h>

extern uint64_t ticks;

uint64_t sleep(uint64_t seconds);

uint64_t sleepms(uint64_t milliseconds);

void tick(struct regs *r);

typedef uint64_t time_t;

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  //_TIME_H
