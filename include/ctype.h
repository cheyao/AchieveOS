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
extern int _ctmp;

#define isalnum(c) (_ctype[c] & (_U | _L | _D))
#define isalpha(c) (_ctype[c] & (_U | _L))
#define iscntrl(c) (_ctype[c] & (_C))
#define isdigit(c) (_ctype[c] & (_D))
#define isgraph(c) (_ctype[c] & (_P | _U | _L | _D))
#define islower(c) (_ctype[c] & (_L))
#define isprint(c) (_ctype[c] & (_P | _U | _L | _D | _SP))
#define ispunct(c) (_ctype[c] & (_P))
#define isspace(c) (_ctype[c] & (_S))
#define isupper(c) (_ctype[c] & (_U))
#define isxdigit(c) (_ctype[c] & (_D | _X))

#define isascii(c) (((unsigned)(c)) <= 0x7f)
#define toascii(c) (((unsigned)(c)) & 0x7f)

#define tolower(c) (_ctmp = (c), isupper(_ctmp) ? _ctmp - ('A' - 'a') : _ctmp)
#define toupper(c) (_ctmp = (c), islower(_ctmp) ? _ctmp - ('a' - 'A') : _ctmp)

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  //_CTYPE_H
