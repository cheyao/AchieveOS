//
// Created by cheyao on 14.09.2022.
//

#include "irq.h"
#include "ports.h"
#include "types.h"

isr_t interrupt_handlers[15];

void register_interrupt_handler(u8int n, isr_t handler) {
    interrupt_handlers[n - 32] = handler;
}

void irq_handler(u8int int_no) {
    if (int_no >= 40) outb(0xA0, 0x20); /* slave */
    outb(0x20, 0x20); /* master */

    if (interrupt_handlers[int_no - 32] != 0) {
        isr_t handler = interrupt_handlers[int_no - 32];
        handler(int_no);
    }
}
