//
// Created by cheyao on 14.09.2022.
//

#ifndef PACMAN_IRQ_H
#define PACMAN_IRQ_H

#include "types.h"

void irq_handler(u8int int_no);

typedef void (*isr_t)();
void register_interrupt_handler(u8int n, isr_t handler);

#endif //PACMAN_IRQ_H
