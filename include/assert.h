//
// Created by cheyao on 27.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _ASSERT_H
#define _ASSERT_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h>

#undef assert
#undef __assert

#ifdef NDEBUG
#define assert(ignore) ((void)0)
#else
#define assert(exp) \
    (__builtin_expect(!(exp), 0) ? error("Assertion failed: %s, function %s, file %s, line %d.", #exp, __func__, __FILE_NAME__, __LINE__) : (void)0)
#endif

#define static_assert _Static_assert

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif //_ASSERT_H
