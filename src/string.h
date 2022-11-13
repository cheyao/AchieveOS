//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _STRING_H
#define _STRING_H

char *itoa(int num, char *buff, int base);

void reverse(char str[], int length);

unsigned long strlen(const char *str);

typedef char *String;

#endif //_STRING_H
