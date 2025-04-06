//
// Created by cheyao on 12.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <stdint.h>
#include <kernel/idt.h>
#include <time.h>

uint64_t ticks;

void tick(struct regs *r) {
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
