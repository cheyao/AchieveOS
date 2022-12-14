//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <stdio.h>

int puts(const char *str) {
	register int i = 0;

	for (; str[i] != 0; ++i)
		putchar(str[i]);

	return i;
}

int putchar(const char c) {
	// uint16_t pos = get_cursor_position();
	//
	// if (c == '\n') {  // New line calculations
	// 	while (pos % 80 != 0) {
	// 		((unsigned char *) BUFFER)[pos * 2] = ' ';
	// 		pos++;
	// 	}
	// } else {
	// 	((unsigned char *) BUFFER)[pos * 2] = c;
	// 	pos++;
	// }
	//
	// update_cursor(pos);
	return c;
}
