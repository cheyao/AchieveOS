//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _STDIO_H
#define _STDIO_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdarg.h>

__attribute__((format(printf, 1, 2))) int printf(const char *restrict format, ...);

int vprintf(const char *restrict format, va_list args);

// int vfprintf(char *stream, const char *restrict format, va_list args);
int putchar(char c);

int puts(const char *restrict str);

extern void clear_screen(void);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  //_STDIO_H
