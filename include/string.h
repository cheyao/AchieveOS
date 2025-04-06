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
#undef NULL
#define NULL ((void *)0)

// TODO: compleate string

size_t strnlen(const char *s, size_t maxlen);

extern void *memcpy(void *restrict dest, const void *restrict src, size_t count);

extern void *memmove(void *dest, const void *src, size_t count);

extern char *strcpy(char *restrict dest, const char *restrict src);

//  char *strncpy(char *restrict s1, const char *restrict s2, size_t

extern char *strcat(char *restrict dest, const char *restrict src);

// char *strncat(char *restrict s1, const char *restrict s2, size_t n);

extern int *memcmp(const void *ptr1, const void *ptr2, size_t count);

extern int strcmp(const char *s1, const char *s2);

#define strcoll(s1, s2) strcmp(s1, s2)
// TODO: locale strcoll
// The strcoll function compares the string pointed to by s1 to the string pointed to by s2, both
// interpreted as appropriate to the LC_COLLATE category of the current locale.

// int strncmp(const char *s1, const char *s2, size_t n);

// size_t strxfrm(char * restrict s1, const char * restrict s2, size_t n);
// The strxfrm function transforms the string pointed to by s2 and places the resulting string into
//the array pointed to by s1. The transformation is such that if the strcmp function is applied to two
//transformed strings, it returns a value greater than, equal to, or less than zero, corresponding to the
//result of the strcoll function applied to the same two original strings. No more than n characters
//are placed into the resulting array pointed to by s1, including the terminating null character. If n is
//zero, s1 is permitted to be a null pointer. If copying takes place between objects that overlap, the
//behavior is undefined.

// void *memchr(const void *s, int c, size_t n);

// char *strchr(const char *s, int c);

extern size_t strcspn(const char *str, const char *set);

// char *strpbrk(const char *s1, const char *s2);

// char *strrchr(const char *s, int c);

extern size_t strspn(const char *str, const char *set);

extern char *strstr(char *haystack, const char *needle);

// char *strtok(char *restrict s1, const char *restrict s2);

extern void *memset(void *dest, int c, size_t count);

// char *strerror(int errnum);

extern size_t strlen(const char *str);

extern int stricmp(const char *string1, const char *string2);

// Some non-standard functions
extern int strcasecmp(const char *string1, const char *string2);

typedef char *String;

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  //_STRING_H
