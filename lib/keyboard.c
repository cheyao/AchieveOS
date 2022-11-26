//
// Created by cheyao on 11.11.2022.
//

#include <kernel/cd.h>
#include <kernel/keyboard.h>
#include <kernel/ports.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

bool volatile stdin = false;
int volatile stdinb;
// char volatile buff[30];
// int volatile index = 0;
// const char chars[] = {'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};

void keyboard_call(void) {
	uint8_t scancode = inb(0x60); /* Da scancode */

	if (scancode >= 0x80)
		return;
}

int gets(void) {
	stdin = true;
	while (stdin);
	return stdinb;
}

void update_cursor(uint16_t pos) {
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t get_cursor_position(void) {
	uint16_t pos = 0;
	outb(0x3D4, 0x0F);
	pos |= inb(0x3D5);
	outb(0x3D4, 0x0E);
	pos |= ((uint16_t) inb(0x3D5)) << 8;
	return pos;
}
