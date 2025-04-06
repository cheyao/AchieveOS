//
// Created by cheyao on 22.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//
#include <stddef.h>
#include <string.h>

size_t strnlen(const char *s, size_t maxlen) {
	const char *sc;
	for (sc = s; maxlen-- && *sc != '\0'; sc++);
	return sc - s;
}
