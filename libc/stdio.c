//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <kernel/keyboard.h>
#include <kernel/screen.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

int intlen(int i, uint8_t base) {
    int l = 1;
    while (i >= base) {
        l++;
        i /= base;
    }
    return l;
}

int vprintf(const char *restrict format, va_list args) { /* Ahh never knew printf is so hard */
    /* the printf isn't finished but enough to use */
    uint16_t pos = get_cursor_position();
    volatile unsigned char *buff = (unsigned char *) BUFFER + pos * 2;
    int i = 0;
    int j = 0;

    for (; format[i] != 0; i++, j++) {
        if (format[i] == '%') {
            i++;

            // Values
            int width = 0;
            bool left = false;
            bool pm = false; /* add the sign */
            bool sp = false; /* space before if no sign */
            bool dot = false; /* contain decimal point even if no small digits*/
            bool zp = false; /* zero padding */
            int precision = 0;
            char length = '\0';

            // Flags
            while (true) {
                switch (format[i]) {
                    case '-':
                        left = true;
                        break;
                    case '+':
                        pm = true;
                        break;
                    case ' ':
                        sp = true;
                        break;
                    case '#':
                        dot = true;
                        break;
                    case '0':
                        zp = true;
                        break;
                    default:
                        goto le;
                }
                i++;
            }
            le:

            // Width
            if (isDigit(format[i])) {
                while (isDigit(format[i])) {
                    width *= 10;
                    width += format[i++] - '0';
                }
            } else if (format[i] == '*') {
                width = va_arg(args, int);
                i++;
            }

            // Precision
            if (format[i] == '.') {
                i++;
                if (isDigit(format[i])) {
                    while (isDigit(format[i])) {
                        precision *= 10;
                        precision += format[i] - '0';
                        i++;
                    }
                } else if (format[i] == '*') {
                    i++;
                    precision = va_arg(args, int);
                }
            }

            // Length
            switch (format[i]) {
                case 'h':
                    length = 'h';
                    if (format[++i] == 'h') {
                        length = 'H';
                        i++;
                    }
                    break;
                case 'l':
                    length = 'l';
                    if (format[++i] == 'l')
                        i++;
                    break;
                case 'j':
                case 'z':
                case 't':
                case 'L':
                    length = format[i++];
                    break;
                default:
                    break;
            }

            // Print
            switch (format[i]) {
                case 'd':
                case 'i': {
                    int str[12];
                    int in = va_arg(args, int);
                    int len = intlen(in, 10);

                    if (width != 0 && width > len)
                        for (int k = width - len; k != 0; k--, j++)
                            buff[j * 2] = zp ? '0' : ' ';

                    itoa(in, str, 10);

                    if (pm && str[0] != '-') {
                        buff[j * 2] = '+';
                        j++;
                    }
                    for (int c = 0; str[c] != 0; c++, j++)
                        buff[j * 2] = str[c];
                    j--;
                    break;
                }
                case '%':
                    buff[j * 2] = '%';
                    break;
                case 'c':
                    buff[j * 2] = va_arg(args, int);
                    break;
                case 's': {
                    char *str = va_arg(args, char *);
                    for (int c = 0; str[c] != 0; c++, j++)
                        buff[j * 2] = str[c];
                    j--;
                    break;
                }
                case 'x': {
                    int str[12];
                    int in;
                    switch (length) {
                        case 'H':
                            in = va_arg(args, int);
                            break;
                        default:
                            in = va_arg(args, unsigned int);
                            break;
                    }
                    int len = intlen(in, 16);

                    if (dot && zp) {
                        buff[j++ * 2] = '0';
                        buff[j++ * 2] = 'x';
                    }

                    if (width != 0 && width > len)
                        for (int k = width - len - (dot ? 2 : 0) - (pm ? 1 : 0); k != 0; k--, j++)
                            buff[j * 2] = zp ? '0' : ' ';

                    if (dot && !zp) {
                        buff[j++ * 2] = '0';
                        buff[j++ * 2] = 'x';
                    }

                    itoa(in, str, 16);

                    if (pm) {
                        buff[j * 2] = '+';
                        j++;
                    }

                    for (int c = 0; str[c] != 0; c++, j++)
                        buff[j * 2] = str[c];
                    j--;
                    break;
                }
                case 'X': {
                    int str[12];
                    int in = va_arg(args, int);
                    int len = intlen(in, 16);

                    if (dot && zp) {
                        buff[j++ * 2] = '0';
                        buff[j++ * 2] = 'X';
                    }

                    if (width > len)
                        for (int k = width - len - (dot ? 2 : 0) - (pm ? 1 : 0); k != 0; k--, j++)
                            buff[j * 2] = zp ? '0' : ' ';

                    if (dot && !zp) {
                        buff[j++ * 2] = '0';
                        buff[j++ * 2] = 'X';
                    }

                    itoa(in, str, 16);

                    if (pm) {
                        buff[j * 2] = '+';
                        j++;
                    }

                    for (int c = 0; str[c] != 0; c++, j++)
                        if (str[c] >= 'a' && str[c] <= 'z')
                            buff[j * 2] = (char) (str[c] - ('a' - 'A'));
                        else
                            buff[j * 2] = str[c];
                    j--;
                    break;
                }
                case 'n':
                    switch (length) {
                        case 'h':
                            *va_arg(args, signed int*) = j;
                            break;
                        case 'H':
                            *va_arg(args, signed char*) = (char) j;
                            break;
                        case 'l':
                        case 'j':
                        case 'z':
                        case 't':
                            /* They are the same, just have different names */
                            *va_arg(args, long int*) = j;
                            break;
                        case '\0':
                        default:
                            *va_arg(args, int*) = j;
                            break;
                    }
                    break;
                case 'b':
                    char str[65];
                    int bin = 0;
                    switch (length) {
                        case 'H':
                            bin = va_arg(args, int);
                            break;
                        default:
                            bin = va_arg(args, unsigned int);
                            break;
                    }
                    int len = intlen(bin, 16);

                    if (len < width)
                        for (int k = width - len - (dot ? 2 : 0) - (pm ? 1 : 0); k != 0; k--, j++)
                            buff[j * 2] = zp ? '0' : ' ';


                    for (int c = 0; str[c] != 0; c++, j++)
                        buff[j * 2] = str[c];
                default:
                    break;
            }
        } else {
            // Print!
            if (format[i] == '\n') { // New line calculations
                do {
                    buff[j * 2] = ' ';
                    j++;
                } while (((uint64_t) buff + j * 2 - BUFFER) % 160 != 158);
            } else buff[j * 2] = format[i]; // Normal print
        }
    }
    update_cursor(pos + j);
    return j;
}


__attribute__ ((format (printf, 1, 2))) int printf(const char *restrict format, ...) {
    va_list list;
    va_start(list, format);
    int i = vprintf(format, list);
    va_end (list);
    return i;
}

void puts(const char *str) {
    uint16_t pos = get_cursor_position();
    int i = 0;

    for (; str[i] != 0; ++pos, ++i)
        ((unsigned char *) BUFFER)[pos * 2] = str[i];

    update_cursor(pos);
}

int putchar(const char c) {
    uint16_t pos = get_cursor_position();
    ((unsigned char *) BUFFER)[pos * 2] = c;
    update_cursor(++pos);
    return c;
}
