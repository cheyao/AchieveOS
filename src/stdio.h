//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _STDIO_H
#define _STDIO_H

#include "types.h"
#include "stdarg.h"

__attribute__ ((format (printf, 1, 2))) int printf(const char *restrict format, ...);

int vprintf(const char *format, va_list args);

int putchar(char c);

void print(const char *restrict str);

#endif //_STDIO_H
