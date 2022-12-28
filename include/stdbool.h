//
// Created by cheyao on 28.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _STDBOOL_H
#define _STDBOOL_H

#if defined(__cplusplus)
extern "C" {
#endif

#undef bool
#undef true
#undef false
#undef __bool_true_false_are_defined

#define bool _Bool
#define true 1
#define false 0
#define __bool_true_false_are_defined 1

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif //_STDBOOL_H
