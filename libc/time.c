//
// Created by cheyao on 12.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include "../include/time.h"

#include "../include/kernel/ports.h"
#include "../include/stdio.h"

uint64_t ticks;

void tick(void) {
    // if (num >= 8) outb(0xA0, 0x20); /* Keep this just so I remember the ports*/
    outb(0x20, 0x20);

    ticks++;
}

uint64_t sleep(uint64_t seconds) {
    uint64_t to_wait = seconds * 100 + ticks;
    while (to_wait != ticks);
    return 0;
}

uint64_t sleepms(uint64_t ms) {
    uint64_t to_wait = ms + ticks;
    while (to_wait > ticks);
    return 0;
}
