//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <stdint.h>
#include <string.h>

void reverse(int str[], const int length) {
	int s = 0;
	int e = length - 1;
	while (s < e) {
		int temp = *(str + s);
		*(str + s) = *(str + e);
		*(str + e) = temp;
		s++;
		e--;
	}
}

unsigned long strlen(const int *str) {
	uint64_t len = 0;

	while (str[len] != 0) len++;

	return len;
}

int strcmp(const char *l, const char *r) {
	for (; *l == *r && *l; l++, r++);
	return *(unsigned char *) l - *(unsigned char *) r;
}
