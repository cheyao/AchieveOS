//
// Created by cheyao on 18.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <kernel/screen.h>

int intlen(int i, uint8_t base) {
	int l = 1;
	while (i >= base) {
		l++;
		i /= base;
	}
	return l;
}

__attribute__((__format__(__printf__, 1, 2)))
int printf(const char *restrict format, ...) {
	va_list list;
	va_start(list, format);
	int i = vprintf(format, list);
	va_end(list);
	return i;
}

void error(const char *restrict format, ...) {
	uint8_t _get_attr(void);
	void _set_attr(uint8_t attr);

	va_list list;
	va_start(list, format);

	char *str = (char *) 0x8000;
	vsprintf(str, format, list);

	uint8_t attr = _get_attr();
	_set_attr(0xCF);
	puts(str);
	_set_attr(attr);
	va_end(list);
}

int _puts(const char *str);

int vprintf(const char *restrict format, va_list args) {
	char *buffer = (char *) 0x8000;
	int i = vsprintf(buffer, format, args);
	_puts(buffer);
	swap_buffers();
	return i;
}

int sprintf(char *str, const char *format, ...) {
	va_list list;
	va_start(list, format);
	int i = vsprintf(str, format, list);
	va_end(list);
	return i;
}
