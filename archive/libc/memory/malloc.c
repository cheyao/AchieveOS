//
// Created by cheyao on 22.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// TODO: Make all these allocs

void *calloc(size_t num, size_t size) {
	void *ptr = malloc(num * size);
	memset(ptr, 0, num * size);
	return ptr;
}

void free(void *ptr) {

}

void *realloc(void *ptr, size_t size) {
	void *new_ptr = malloc(size);
	if (new_ptr == NULL)
		return NULL;
	// memcpy(new_ptr, ptr);
	free(ptr);

	return new_ptr;
}

void *malloc(size_t size) {
	if (size == 0)
		return NULL;
	return NULL;
}
