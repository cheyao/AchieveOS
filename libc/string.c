//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

void reverse(int str[], int length) {
    int s = 0;
    int e = length - 1;
    while (s < e) {
        int temp = *(str + s);
        *(str + s) = *(str + e);
        *(str + e) = temp;
        s++;
        e--;
    }
}

int *itoa(int num, int *buff, int base) {
    register int i = 0;
    bool n = false;

    if (num == 0) {
        buff[i] = '0';
        buff[++i] = '\0';
        return buff;
    }

    if (num < 0 && base == 10) {
        n = true;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        buff[i++] = rem > 9 ? rem - 10 + 'a' : rem + '0';
        num = num / base;
    }

    if (n)
        buff[i++] = '-';

    buff[i] = '\0';

    reverse(buff, i);

    return buff;
}

unsigned long strlen(const int *str) {
    uint64_t len = 0;

    while (str[len] != 0) len++;

    return len;
}
