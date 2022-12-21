//
// Created by cheyao on 12.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _CTYPE_H
#define _CTYPE_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h>

bool isdigit(char i);

extern void tolower(char *string);

extern void toupper(char *string);

int isspace(int c);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  //_CTYPE_H
