//
// Created by cheyao on 12.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _STDLIB_H
#define _STDLIB_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#define ____PAGE_TABLE 0x500000

char *itoa(int num, char *buff, int base);

int atoi(const char *str);

static inline uint32_t rand(void) {
	uint32_t r;
	__asm__ __volatile__("rdrand %0"
			: "=a"(r));
	return r;
}

void *calloc(size_t num, size_t size);

void free(void *ptr);

void *realloc(void *ptr, size_t size);

void *malloc(size_t size);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  //_STDLIB_H
