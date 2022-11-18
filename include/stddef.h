//
// Created by cheyao on 18.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _STDDEF_H
#define _STDDEF_H

#undef NULL
#define NULL 0

#define offsetof(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)

typedef long long ptrdiff_t;
typedef long unsigned int size_t;

#endif //_STDDEF_H
