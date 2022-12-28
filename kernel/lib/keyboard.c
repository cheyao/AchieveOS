//
// Created by cheyao on 11.11.2022.
//

#include <kernel/keyboard.h>  // for get_cursor_position, keyboard_call, upd...
#include <kernel/ports.h>     // for outb, inb
#include <stdbool.h>          // for bool, false, true
#include <stdint.h>           // for uint16_t, uint8_t
#include <stdio.h>           // for uint16_t, uint8_t
#include <kernel/idt.h>

// char volatile buff[30];
// int volatile index = 0;

typedef struct {
	uint16_t shift: 1,
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
			esc: 1;
} _keys;

void keyboard_call(struct regs *r) {
	uint8_t scancode = inb(0x60); /* Da scancode */

	if (scancode >= 0x80)
		return;

	putchar(scancode);
}
