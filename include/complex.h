//
// Created by cheyao on 27.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _COMPLEX_H
#define _COMPLEX_H

#if defined(__cplusplus)
extern "C" {
#endif

#undef complex
#undef _Complex_I
#undef I

#define complex _Complex
#define _Complex_I (__extension__ 1.0iF)
// GCC doesn't support imaginary values. (Who even knows that they existed)
#define I _Complex_I

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif //_COMPLEX_H
