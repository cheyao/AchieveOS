//
// Created by cheyao on 25.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _MEMORY_H
#define _MEMORY_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stddef.h>

// TODO: finish the structs

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

void __init_memory(void);

void *__alloc_page(size_t pages);

void __free_page(void *start, size_t pages);

void *__alloc_virt_page(size_t pages);

void __free_virt_page(void *start, size_t pages);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif //_MEMORY_H
