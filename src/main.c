#include "types.h"

static u32int ticks = 0;

void mainC() {
    BochsBreak()
    **((unsigned char **) (0x7F000 + 0x28)) = 6;
}

void tickC() {
    ticks++;
}