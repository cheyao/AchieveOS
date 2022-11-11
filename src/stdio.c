//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include "stdio.h"
#include "keyboard.h"
#include "types.h"

void printf(const char *restrict format, ...) {
}

void print(const char *restrict str) {
    u16int pos = get_cursor_position();

    for (; *str != 0; pos++, str++)
        ((unsigned char *) BUFFER)[pos * 2] = *str;

    update_cursor(pos);
}

void putchar(const char c) {
    u16int pos = get_cursor_position();
    ((unsigned char *) BUFFER)[pos * 2] = c;
    update_cursor(++pos);
}
