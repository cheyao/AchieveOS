//
// Created by cheyao on 10.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _MISC_H
#define _MISC_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stddef.h>

int InstructionSet(void);

int UnalignedIsFaster(void);

size_t DataCacheSize(int level);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  // _MISC_H
