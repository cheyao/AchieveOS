//
// Created by cheyao on 08.10.2022.
//

#ifndef PACMAN_TYPES_H
#define PACMAN_TYPES_H

#include "ports.h"

typedef unsigned long long u64int;
typedef long long s64int;
typedef unsigned int u32int;
typedef int s32int;
typedef unsigned short u16int;
typedef short s16int;
typedef unsigned char u8int;
typedef char s8int;

// OSdev wiki style :P
typedef unsigned long long uint64_t;
typedef long long int64_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned char uint8_t;
typedef char int8_t;
typedef signed long long intmax_t;
typedef _Bool bool;

#define BUFFER 0xB8000
#define WIDTH 640
#define HEIGHT 400
#define NULL 0
#define BREAK asm volatile ("1: jmp 1b");

//outputs a character to the debug console
#define BochsConsolePrintChar(c) outportb(0xe9, c)
//stops simulation and breaks into the debug console
#define BochsBreak() outw(0x8A00,0x8A00); outw(0x8A00,0x08AE0);
#define BochsMagicBreak() __asm__ __volalite__ ("xchg $bx, $bx");

#define EOF 0xFF

#define True 1
#define False 0
#define true 1
#define false 0

#endif //PACMAN_TYPES_H
