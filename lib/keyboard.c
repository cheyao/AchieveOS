//
// Created by cheyao on 11.11.2022.
//

#include <kernel/keyboard.h>  // for get_cursor_position, keyboard_call, upd...
#include <kernel/ports.h>     // for outb, inb
#include <stdbool.h>          // for bool, false, true
#include <stdint.h>           // for uint16_t, uint8_t
#include <stdio.h>           // for uint16_t, uint8_t
#include <kernel/idt.h>
#include <ctype.h>

// char volatile buff[30];
// int volatile index = 0;

struct {
	uint32_t shift: 1,
			caps_lock: 1,
			command: 1,
			option: 1,
			control: 1,
			fn1: 1,
			fn2: 1,
			fn3: 1,
			fn4: 1,
			fn5: 1,
			fn6: 1,
			fn7: 1,
			fn8: 1,
			fn9: 1,
			fn10: 1,
			fn11: 1,
			fn12: 1,
			esc: 1,
			e0: 1,
			f0: 1;
} keys;

static const char chars[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, '\t', '`', 0, 0, 0, 0, 0, 0,
		'q', '1', 0, 0, 0, 'z', 's', 'a', 'w',
		'2', 0, 0, 'c', 'x', 'd', 'e', '4', '3',
		0, 0, ' ', 'v', 'f', 't', 'r', '5', 0,
		0, 'n', 'b', 'h', 'g', 'y', '6', 0, 0,
		0, 'm', 'j', 'u', '7', '8', 0, 0, ',', 'k',
		'i', 'o', '0', 0, 0, '.', '/', 'l',
		'l', ';', 'p', '-', 0, 0, 0, '\'', 0,
		'[', '=', 0, 0, 0, 0, '\n', ']', 0,
		'\\', 0, 0, 0, 0, 0, 0, 0, 0, '\b',
		0, 0, '1', 0, '4', '7', 0, 0, 0,
		'0', '.', '2', '5', '6', '8', '\033', 0,
		0, '+', '3', '-', '*', '9', 0, 0, 0,
		0, 0, 0, 0
};

static const char upper_chars[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, '\t', '~', 0, 0, 0, 0, 0, 0,
		'Q', '!', 0, 0, 0, 'Z', 'S', 'A', 'W',
		'@', 0, 0, 'C', 'X', 'D', 'E', '$', '#',
		0, 0, ' ', 'V', 'F', 'T', 'R', '%', 0,
		0, 'N', 'B', 'H', 'G', 'Y', '^', 0, 0,
		0, 'M', 'J', 'U', '&', '*', 0, 0, '<', 'K',
		'I', 'O', ')', 0, 0, '>', '?', 'L',
		'L', ':', 'P', 0, 0, 0, '\"', 0, 0,
		'{', '+', 0, 0, 0, 0, '\n', '}', 0,
		'|', 0, 0, 0, 0, 0, 0, 0, 0, '\b',
		0, 0, '1', 0, '4', '7', 0, 0, 0,
		'0', '.', '2', '5', '6', '8', '\033', 0,
		0, '+', '3', '-', '*', '9', 0, 0, 0,
		0, 0, 0, 0
};

void keyboard_call(struct regs *r) {
	uint8_t scancode = inb(0x60); /* Da scancode */

	if (scancode == 0xe0) {
		keys.e0 = 1;
		return;
	}

	if (scancode == 0xF0) {
		keys.f0 = 1;
		return;
	}

	if (keys.e0 && keys.f0) {
		keys.f0 = keys.e0 = 0;
		return;
	}

	if (keys.f0) {
		keys.f0 = 0;

		if (scancode == 0x12 || scancode == 0x59) {
			keys.shift = 0;
			return;
		}
		return;
	}

	if (scancode == 0x12 || scancode == 0x59) {
		keys.shift = 1;
		return;
	}

	if (scancode == 0x58) {
		keys.caps_lock = !keys.caps_lock;
		return;
	}

	char c;
	if (keys.shift || keys.caps_lock)
		c = upper_chars[scancode];
	else
		c = chars[scancode];

	putchar(c);
}
