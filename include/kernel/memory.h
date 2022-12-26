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

void __init_memory(void);

void *__alloc_page(size_t pages);

void __free_page(void *start, size_t pages);

void *__alloc_virt_page(size_t pages);

void __free_virt_page(void *start, size_t pages);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif //_MEMORY_H
