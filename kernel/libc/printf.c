#include <ctype.h>
#include <hedley.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
	int i = 0;
	int j = 0;

	for (; format[i] != 0; i++, j++) {
		if (format[i] == '%') {
			i++;

			// Values
			int width = 0;
			__attribute__((unused)) bool left = false;
			bool pm = false;
			__attribute__((unused)) bool sp = false;
			bool dot = false;
			bool zp = false;
			__attribute__((unused)) int precision = 0;
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
			if (isdigit(format[i])) {
				while (isdigit(format[i])) {
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
				if (isdigit(format[i])) {
					while (isdigit(format[i])) {
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
				default:
					break;
			}

			// Print
			switch (format[i]) {
				case 'd':
				case 'i': {
					char str[12];
					int in = va_arg(args, int);

					itoa(in, str, 10);

					for (int c = 0; str[c] != 0; c++, j++)
						putchar(str[c]);
					j--;
					break;
				}
				case '%':
					putchar('%');
					break;
				case 'c':
					putchar(va_arg(args, int));
					break;
				case 's': {
					char *str = va_arg(args, char *);
					for (int c = 0; str[c] != 0; c++, j++)
						putchar(str[c]);
					j--;
					break;
				}
				case 'x': {
					char str[12];
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
						putchar('0');
						putchar('x');
					}

					if (width != 0 && width > len)
						for (int k = width - len - (dot ? 2 : 0) - (pm ? 1 : 0); k != 0; k--, j++)
							putchar(zp ? '0' : ' ');

					if (dot && !zp) {
						putchar('0');
						putchar('x');
					}

					itoa(in, str, 16);

					if (pm) {
						putchar('+');
						j++;
					}

					for (int c = 0; str[c] != 0; c++, j++)
						putchar(str[c]);
					j--;
					break;
				}
				case 'X': {
					char str[12];
					int in = va_arg(args, int);
					int len = intlen(in, 16);

					if (dot && zp) {
						putchar('0');
						putchar('X');
					}

					if (width > len)
						for (int k = width - len - (dot ? 2 : 0) - (pm ? 1 : 0); k != 0; k--, j++)
							putchar(zp ? '0' : ' ');

					if (dot && !zp) {
						putchar('0');
						putchar('X');
					}

					itoa(in, str, 16);

					if (pm) {
						putchar('+');
						j++;
					}

					for (int c = 0; str[c] != 0; c++, j++)
						if (str[c] >= 'a' && str[c] <= 'z')
							putchar((char) (str[c] - ('a' - 'A')));
						else
							putchar(str[c]);
					j--;
					break;
				}
				case 'n':
					switch (length) {
						case 'h':
							*va_arg(args, signed int *) = j;
							break;
						case 'H':
							*va_arg(args, signed char *) = (char) j;
							break;
						case 'l':
						case 'j':
						case 'z':
						case 't':
							/* They are the same, just have different names */
							*va_arg(args, long int *) = j;
							break;
						case '\0':
						default:
							*va_arg(args, int *) = j;
							break;
					}
					break;
				case 'b': {
					char str[65] = {0};
					int bin;
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
							putchar(zp ? '0' : ' ');

					for (int c = 0; str[c] != 0; c++, j++)
						putchar(str[c]);
				}
				default:
					break;
			}
		} else {
			// Print!
			putchar(format[i]);  // Normal print
		}
	}
	return j;
}

HEDLEY_PRINTF_FORMAT(1, 2)
int printf(const char *restrict format, ...) {
	va_list list;
	va_start(list, format);
	int i = vprintf(format, list);
	va_end(list);
	return i;
}