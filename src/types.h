//
// Created by cheyao on 08.10.2022.
//

#ifndef _TYPES_H
#define _TYPES_H

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
typedef _Bool bool;

typedef void all;

typedef int wint_t;
typedef char wchar_t;

typedef unsigned long long uintmax_t;
typedef long long intmax_t;
typedef unsigned long long uintptr_t;
typedef unsigned long long intptr_t;

typedef unsigned long long uint_least64_t;
typedef long long int_least64_t;
typedef unsigned int uint_least32_t;
typedef int int_least32_t;
typedef unsigned short uint_least16_t;
typedef short int_least16_t;
typedef unsigned char uint_least8_t;
typedef char int_least8_t;
typedef unsigned long long uint_fast64_t;
typedef long long int_fast64_t;
typedef unsigned int uint_fast32_t;
typedef int int_fast32_t;
typedef unsigned short uint_fast16_t;
typedef short int_fast16_t;
typedef unsigned char uint_fast8_t;
typedef char int_fast8_t;

typedef unsigned long size_t;
typedef long ptrdiff_t;

#define BUFFER 0xB8000
#define WIDTH 80
#define HEIGHT 25
#define NULL 0
#define BREAK asm volatile ("1: jmp 1b");

#define INTMAX_MIN (-(2^63-1))

//stops simulation and breaks into the debug console
#define BochsBreak() outw(0x8A00,0x8A00); outw(0x8A00,0x08AE0);
#define BochsMagicBreak() __asm__ __volalite__ ("xchg $bx, $bx");

#define EOF 0xFF

#define True 1
#define False 0
#define true 1
#define false 0

typedef s32int pid_t;
typedef u32int uid_t;
typedef u32int gid_t;

#endif //_TYPES_H
