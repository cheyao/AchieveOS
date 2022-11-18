//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h>

__attribute__ ((format (printf, 1, 2))) int printf(const char *restrict format, ...);

int vprintf(const char *restrict format, va_list args);

// int vfprintf(char *stream, const char *restrict format, va_list args);
int putchar(char c);

void puts(const char *restrict str);

extern void clear_screen(void);

typedef struct {
    char *addr;
    char *read_to;
} FILE;

static __attribute__((unused)) FILE *const stderr = (FILE *) -1;


#endif //_STDIO_H
