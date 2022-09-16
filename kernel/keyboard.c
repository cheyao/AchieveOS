//
// Created by cheyao on 14.09.2022.
//

#include "idt.h"
#include "irq.h"
#include "keyboard.h"
#include "ports.h"



static void keyboard_callback() {
    unsigned char scancode = inb(0x60);

    switch (scancode) {
    }
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}
