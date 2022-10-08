//
// Created by cheyao on 08.10.2022.
//

#include "ports.h"

#ifndef PACMAN_TYPES_H
#define PACMAN_TYPES_H

typedef unsigned int u32int;
typedef int s32int;
typedef unsigned short u16int;
typedef short s16int;
typedef unsigned char u8int;
typedef char s8int;

#define FRAMEBUFFER 0xFD000000
#define WIDTH 640
#define HEIGHT 400
#define NULL 0
#define BREAK asm volatile ("1: jmp 1b");

//outputs a character to the debug console
#define BochsConsolePrintChar(c) outportb(0xe9, c)
//stops simulation and breaks into the debug console
#define BochsBreak() outw(0x8A00,0x8A00); outw(0x8A00,0x08AE0);

#endif //PACMAN_TYPES_H
