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

void reverse(int str[], int length);

unsigned long strlen(const int *str);

void *memcpy(void *destination, const void *source, size_t num);

typedef char *String;

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  //_STRING_H
