//
// Created by cheyao on 12.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int *itoa(int num, int *buff, int base) {
	register int i = 0;
	bool n = false;

	if (num == 0) {
		buff[i] = '0';
		buff[++i] = '\0';
		return buff;
	}

	if (num < 0 && base == 10) {
		n = true;
		num = -num;
	}

	while (num != 0) {
		int rem = num % base;
		buff[i++] = rem > 9 ? rem - 10 + 'a' : rem + '0';
		num = num / base;
	}

	if (n)
		buff[i++] = '-';

	buff[i] = '\0';

	reverse(buff, i);

	return buff;
}

int atoi(const char *str) {
	int n = 0;
	int neg = 0;
	while (isspace(*str)) {
		str++;
	}
	switch (*str) {
		case '-':
			neg = 1;
			__attribute__((fallthrough));
		case '+':
			str++;
	}
	while (isDigit(*str)) {
		n = 10 * n - (*str++ - '0');
	}

	return neg ? n : -n;
}
