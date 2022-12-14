//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _STRING_H
#define _STRING_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stddef.h>

size_t strlen(const char *str);

// From asmlib:
extern void *__memset(void *dest, int c, size_t count);

#define memset __memset

extern void *__memcpy(void *dest, const void *src, size_t count);

#define memcpy __memcpy

typedef char *String;

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  //_STRING_H
