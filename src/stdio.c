//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include "stdio.h"
#include "keyboard.h"
#include "types.h"
#include "stdarg.h"

int vprintf(const char *format, va_list args) {
    print(format);
    // TODO: finish printf
    return 0;
}


__attribute__ ((format (printf, 1, 2))) int printf(const char *restrict format, ...) {
    va_list list;
    va_start(list, format);
    int i = vprintf(format, list);
    va_end (list);
    return i;
}

void print(const char *restrict str) {
    u16int pos = get_cursor_position();

    for (; *str != 0; pos++, str++)
        ((unsigned char *) BUFFER)[pos * 2] = *str;

    update_cursor(pos);
}

int putchar(const char c) {
    u16int pos = get_cursor_position();
    ((unsigned char *) BUFFER)[pos * 2] = c;
    update_cursor(++pos);
    return c;
}
