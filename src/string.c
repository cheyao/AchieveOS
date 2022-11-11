//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include "string.h"

void itoa(int i, char *buffer) {
    for (; i >= 0; i /= 10) {
        buffer[i] = i % 10 + '0';
    }
}
