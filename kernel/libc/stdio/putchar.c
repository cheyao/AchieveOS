//
// Created by cheyao on 21.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <ctype.h>
#include <kernel/screen.h>
#include <kernel/terminal-font.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t __cursor_x = 0;
static uint32_t __cursor_y = 0;
static uint8_t __attr = 0x0F;

void _set_attr(uint8_t attr) {
	__attr = attr;
}

uint8_t _get_attr(void) {
	return __attr;
}

#define CHARACTER_PER_LINE 128
#define LINE_PER_PAGE 45

int puts(const char *str) {
	int i = 0;

	for (; str[i] != 0; i++) {
		if (str[i] == 0x1B && str[++i] == '[') {
			unsigned int j = 0, k = 0;
			i++;
			if (isdigit((int) str[i])) {  // First digit
				j = atoi(str);

				while (isdigit((int) str[i]))
					i++;

				if (str[i] == ';') {
					i++;  // Second digit
					if (isdigit((int) str[i])) {
						k = atoi(str);

						while (isdigit((int) str[i]))
							i++;
					}
				}
			}

			switch (str[i]) {
				case 'J': {
					__cursor_x = j > 0 ? j : 1;
					__cursor_y = k > 0 ? k : 1;
				}
			}
		} else {
			putchar(str[i]);
		}
	}

	putchar('\n');

	return i;
}

static uint32_t term_colors[] = {
		rgb(000, 000, 000),
		rgb(000, 000, 238),
		rgb(000, 205, 000),
		rgb(000, 205, 205),
		rgb(205, 000, 000),
		rgb(205, 000, 205),
		rgb(205, 205, 000),
		rgb(229, 229, 229),
		rgb(127, 127, 127),
		rgb(92, 92, 255),
		rgb(000, 255, 000),
		rgb(000, 255, 255),
		rgb(255, 000, 000),
		rgb(255, 000, 255),
		rgb(255, 255, 000),
		rgb(255, 255, 255)};

int putchar(const int c) {
	uint32_t colors[] = {term_colors[(__attr >> 4) & 0xF], term_colors[__attr & 0xF]};

	uint8_t *font = __terminal_font[c];

	switch (c) {
		case '\b': {
			if (__cursor_x != 0)
				__cursor_x--;

			for (int i = 0; i < LARGE_FONT_CELL_HEIGHT; i++) {
				for (int j = 0; j < LARGE_FONT_CELL_WIDTH; j++) {
					putPixel(__cursor_x * LARGE_FONT_CELL_WIDTH + j, __cursor_y * LARGE_FONT_CELL_HEIGHT + i,
					         colors[0]);
				}
			}

			break;
		}
		case 0x0D: {
			__cursor_x = 0;
			__cursor_y = 0;
			break;
		}
		case 0x09: {
			__cursor_x += __cursor_x % 8 ? __cursor_x / 8 : 0;
			if (__cursor_x >= CHARACTER_PER_LINE) {
				__cursor_x = 0;
				__cursor_y++;
			}
			break;
		}
		case '\n': {
			for (; __cursor_x != 0;) {
				for (int i = 0; i < LARGE_FONT_CELL_HEIGHT; i++) {
					for (int j = 0; j < LARGE_FONT_CELL_WIDTH; j++) {
						putPixel(__cursor_x * LARGE_FONT_CELL_WIDTH + j, __cursor_y * LARGE_FONT_CELL_HEIGHT + i,
						         colors[0]);
					}
				}

				__cursor_x++;
				if (__cursor_x >= CHARACTER_PER_LINE) {
					__cursor_x = 0;
					__cursor_y++;
				}
			}
			break;
		}
		default: {
			for (int i = 0; i < LARGE_FONT_CELL_HEIGHT; i++) {
				for (int j = 0; j < LARGE_FONT_CELL_WIDTH; j++) {
					putPixel(__cursor_x * LARGE_FONT_CELL_WIDTH + j, __cursor_y * LARGE_FONT_CELL_HEIGHT + i,
					         colors[(font[i] & (1 << (LARGE_FONT_MASK - j))) != 0]);
				}
			}

			__cursor_x++;
			if (__cursor_x >= CHARACTER_PER_LINE) {
				__cursor_x = 0;
				__cursor_y++;
			}

			break;
		}
	}

	return c;
}
