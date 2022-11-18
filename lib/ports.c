//
// Created by cheyao on 08.10.2022.
//

#include "../include/kernel/ports.h"

unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ __volatile__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void outb(unsigned short port, unsigned char data) {
    __asm__ __volatile__("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short inw(unsigned short port) {
    unsigned short result;
    __asm__ __volatile__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void outw(unsigned short port, unsigned short data) {
    __asm__ __volatile__("out %%ax, %%dx" : : "a" (data), "d" (port));
}