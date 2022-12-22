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
#include <hedley.h>
#include <stddef.h>

typedef struct FILE {
	char *path;

	void (*putchar)(int c);
} FILE;

static __attribute__((unused)) FILE *const stdin = (FILE *const) -1; // Mhm ima just let the fprintf check
static __attribute__((unused)) FILE *const stdout = (FILE *const) -2;
static __attribute__((unused)) FILE *const stderr = (FILE *const) -3;

HEDLEY_PRINTF_FORMAT(1, 2) int printf(const char *format, ...); // Implemented
int vprintf(const char *format, va_list arg); // Implemented
// HEDLEY_PRINTF_FORMAT(2, 3) int fprintf(FILE *stream, const char *format, ...);
// int vfprintf(FILE *stream, const char *format, va_list arg);
HEDLEY_PRINTF_FORMAT(2, 3) int sprintf(char *str, const char *format, ...);

int vsprintf(char *s, const char *format, va_list arg);
// HEDLEY_PRINTF_FORMAT(3, 4) int snprintf( char *s, size_t n, const char * format, ...);
// int vsnprintf(char *s, size_t n, const char *format, va_list arg );

// int vfprintf(char *stream, const char *restrict format, va_list args);
int putchar(int c);

#define EOF -1

int puts(const char *restrict str);

extern void clear_screen(void);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  //_STDIO_H
