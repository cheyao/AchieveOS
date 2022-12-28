//
// Created by cheyao on 27.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _STDDEF_H
#define _STDDEF_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef long ptrdiff_t;
typedef unsigned long size_t;
typedef struct {
	long long __max_align_ll __attribute__((__aligned__(__alignof__(long long))));
	long double __max_align_ld __attribute__((__aligned__(__alignof__(long double))));
} max_align_t;
typedef __WCHAR_TYPE__ wchar_t; // TODO: replace this with portable wchar_t

#define NULL ((void *)0)
#define offsetof(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER) // TODO: replace this


#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif //_STDDEF_H
