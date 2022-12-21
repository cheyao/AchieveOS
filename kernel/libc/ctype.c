//
// Created by cheyao on 13.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <ctype.h>
#include <stdbool.h>

bool isdigit(char i) {
	return (i >= '0') & (i <= '9');
}

int isspace(int c) {
	return (c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == ' ');
}
