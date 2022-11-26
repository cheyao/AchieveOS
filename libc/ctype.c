//
// Created by cheyao on 13.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <ctype.h>
#include <stdbool.h>

bool isDigit(char i) {
	return (i >= '0') & (i <= '9');
}

int tolower(int ch) {
	if (ch >= 'A' && ch <= 'Z') {
		return ch - 'A' + 'a';
	}
	return ch;
}

int isspace(int c) {
	return (c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == ' ');
}