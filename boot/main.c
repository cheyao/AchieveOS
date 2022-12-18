//
// Created by cheyao on 14.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//
#include <kernel/screen.h>
#include <stdint.h>

void main(void) {
	*((uint16_t *) BUFFER) = rgb(0xFF, 0xFF, 0xFF);
}
