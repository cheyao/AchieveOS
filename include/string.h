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

typedef __SIZE_TYPE__ size_t;
#define NULL ((void *)0)

// From asmlib:
extern void *memset(void *dest, int c, size_t count);

extern void *memcpy(void *dest, const void *src, size_t count);

extern int *memcmp(const void *ptr1, const void *ptr2, size_t count);

extern void *memmove(void *dest, const void *src, size_t count);

extern char *strcpy(char *dest, const char *src);

extern size_t strlen(const char *str);

extern char *strcat(char *dest, const char *src);

extern int strcmp(const char *s1, const char *s2);

extern size_t strspn(const char *str, const char *set);

extern size_t strcspn(const char *str, const char *set);

extern char *strstr(char *haystack, const char *needle);

// Some non-standard functions
extern int strcasecmp(const char *string1, const char *string2);

typedef char *String;

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  //_STRING_H
