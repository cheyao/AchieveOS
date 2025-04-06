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

// From asmlib
extern void strtolower(char *string);
extern void strtoupper(char *string);

#define _U 0x01  /* upper */
#define _L 0x02  /* lower */
#define _D 0x04  /* digit */
#define _C 0x08  /* cntrl */
#define _P 0x10  /* punct */
#define _S 0x20  /* white space (space/lf/tab) */
#define _X 0x40  /* hex digit */
#define _SP 0x80 /* hard space (0x20) */

extern int _ctype[];

// TODO: implement locale
int static inline isalnum(int c) { return (_ctype[c] & (_U | _L | _D)); }

int static inline isalpha(int c) { return (_ctype[c] & (_U | _L)); }

int static inline iscntrl(int c) { return (_ctype[c] & (_C)); }

int static inline isdigit(int c) { return (_ctype[c] & (_D)); }

int static inline isgraph(int c) { return (_ctype[c] & (_P | _U | _L | _D)); }

int static inline islower(int c) { return (_ctype[c] & (_L)); }

int static inline isprint(int c) { return (_ctype[c] & (_P | _U | _L | _D | _SP)); }

int static inline ispunct(int c) { return (_ctype[c] & (_P)); }

int static inline isspace(int c) { return (_ctype[c] & (_S)); }

int static inline isupper(int c) { return (_ctype[c] & (_U)); }

int static inline isxdigit(int c) { return (_ctype[c] & (_D | _X)); }

int static inline tolower(int c) { return isupper(c) ? c - ('A' - 'a') : c; }

int static inline toupper(int c) { return islower(c) ? c - ('a' - 'A') : c; }


#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  //_CTYPE_H
