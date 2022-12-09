//
// Created by cheyao on 09.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _ASSERT_H
#define _ASSERT_H

#if defined(__cplusplus)
extern "C" {
#endif

extern void __assert_func(const char *file, int line, const char *func, const char *failedexpr);

#define assert(statement) ((statement) ? (void)0 : __assert_func(__FILE__, __LINE__, __func__, #statement))

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  //_ASSERT_H
