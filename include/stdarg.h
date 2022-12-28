//
// Created by cheyao on 28.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _STDARG_H
#define _STDARG_H

#if defined(__cplusplus)
extern "C" {
#endif

#undef va_start
#undef va_end
#undef va_arg
#undef va_copy

typedef __builtin_va_list va_list;

#define va_start(v, l) __builtin_va_start(v, l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v, l) __builtin_va_arg(v, l)
#define va_copy(d, s) __builtin_va_copy(d, s)

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif //_STDARG_H
