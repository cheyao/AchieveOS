//
// Created by cheyao on 18.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <hedley.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

int intlen(int i, uint8_t base) {
	int l = 1;
	while (i >= base) {
		l++;
		i /= base;
	}
	return l;
}

HEDLEY_PRINTF_FORMAT(1, 2)
int printf(const char *restrict format, ...) {
	va_list list;
	va_start(list, format);
	int i = vprintf(format, list);
	va_end(list);
	return i;
}

int vprintf(const char *restrict format, va_list args) {
	char *buffer = (char *) 0x8000;
	int i = vsprintf(buffer, format, args);
	puts(buffer);
	return i;
}

int sprintf(char *str, const char *format, ...) {
	va_list list;
	va_start(list, format);
	int i = vsprintf(str, format, list);
	va_end(list);
	return i;
}
