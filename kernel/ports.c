//
// Created by cheyao on 13.09.2022.
//

#include "types.h"

unsigned char inb (u16int port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void outb (u16int port, u8int data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}
