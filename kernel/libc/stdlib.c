//
// Created by cheyao on 12.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void reverse(char *str) {
	size_t n = strlen(str);

	for (size_t i = 0; i < n / 2; i++) {
		char temp = str[i];
		str[i] = str[n - i - 1];
		str[n - i - 1] = temp;
	}
}

char *itoa(int num, char *str, int base) {
	int i = 0;
	bool isNegative = false;

	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}

	if (num < 0 && base == 10) {
		isNegative = true;
		num = -num;
	}

	while (num != 0) {
		int rem = num % base;

		str[i++] = rem > 9 ? rem - 10 + 'a' : rem + '0';
		num = num / base;
	}

	if (isNegative)
		str[i++] = '-';

	str[i] = '\0';

	reverse(str);

	return str;
}

int atoi(const char *str) {
	int n = 0;
	int neg = 0;
	while (isspace((int) *str)) {
		str++;
	}
	switch (*str) {
		case '-':
			neg = 1;
			__attribute__((fallthrough));
		case '+':
			str++;
	}
	while (isdigit((int) *str)) {
		n = 10 * n - (*str++ - '0');
	}

	return neg ? n : -n;
}
