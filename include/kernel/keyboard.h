//
// Created by cheyao on 11.11.2022.
//

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void update_cursor(uint16_t pos);

void keyboard_call(void);

uint16_t get_cursor_position(void);

#endif //KEYBOARD_H
