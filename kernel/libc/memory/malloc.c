//
// Created by cheyao on 22.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

//

void *calloc(size_t num, size_t size) {
	void *ptr = malloc(num * size);
	memset(ptr, 0, num * size);
	return ptr;
}

void free(void *ptr) {

}

typedef struct {
	uint32_t present: 1,
			rw: 1,
			us: 1,
			pwt: 1,
			pcd: 1,
			a: 1,
			d: 1,
			pat: 1,
			g: 1,
			avl: 3,
			frame_addr: 20;
} ____page_entry;

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
