//
// Created by cheyao on 12.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _STDLIB_H
#define _STDLIB_H

#if defined(__cplusplus)
extern "C" {
#endif

char *itoa(int num, char *buff, int base);

int atoi(const char *str);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  //_STDLIB_H
