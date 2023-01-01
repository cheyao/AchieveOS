//
// Created by cheyao on 11.11.2022.
//

#ifndef KEYBOARD_H
#define KEYBOARD_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <kernel/idt.h>

void keyboard_call(struct regs *r);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  // KEYBOARD_H
