//
// Created by cheyao on 18.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _STDINT_H
#define _STDINT_H

// Standard types
// Thanks for the reference https://en.cppreference.com/w/cpp/header/cstdint
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

typedef long long int_fast8_t;
typedef long long int_fast16_t;
typedef long long int_fast32_t;
typedef long long int_fast64_t;

typedef char int_least8_t;
typedef short int_least16_t;
typedef int int_least32_t;
typedef long long int_least64_t;

typedef long long intmax_t;
typedef long long intptr_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef unsigned long long uint_fast8_t;
typedef unsigned long long uint_fast16_t;
typedef unsigned long long uint_fast32_t;
typedef unsigned long long uint_fast64_t;

typedef unsigned char uint_least8_t;
typedef unsigned short uint_least16_t;
typedef unsigned int uint_least32_t;
typedef unsigned long long uint_least64_t;

typedef unsigned long long uintmax_t;
typedef unsigned long long uintptr_t;

typedef int32_t pid_t;
typedef uint32_t uid_t;
typedef uint32_t gid_t;

#endif //_STDINT_H
