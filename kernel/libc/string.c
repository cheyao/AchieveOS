//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <string.h>

size_t strlen(const char *str) {
	size_t len = 0;

	while (str[len] != 0) len++;

	return len;
}

int strcmp(const char *l, const char *r) {
	for (; *l == *r && *l; l++, r++);
	return *(unsigned char *) l - *(unsigned char *) r;
}
