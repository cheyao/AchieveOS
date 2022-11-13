//
// Created by cheyao on 12.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include "time.h"
#include "ports.h"
#include "stdio.h"

u64int ticks;

void tick(void) {
    // if (num >= 8) outb(0xA0, 0x20);
    outb(0x20, 0x20);

    ticks++;
}

u32int sleep(u64int seconds) {
    u64int to_wait = seconds * 100 + ticks;
    while (to_wait != ticks);
    return 0;
}
