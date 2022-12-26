//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <kernel/idt.h>
#include <kernel/keyboard.h>
#include <kernel/ports.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

__attribute__((aligned(0x10))) static idt_t idt;
static isr_t idt_entry[256];
irq_t interrupt_handlers[16];

void __set_gate(uint8_t vector, void (*isr)(void)) {
	isr_t *descriptor = &idt_entry[vector];

	descriptor->isr_low = (uint64_t) isr & 0xFFFF;
	descriptor->cs = 0x8;
	descriptor->ist = 0;
	descriptor->attributes = 0x8E;
	descriptor->isr_mid = ((uint64_t) isr >> 16) & 0xFFFF;
	descriptor->isr_high = ((uint64_t) isr >> 32) & 0xFFFFFFFF;
	descriptor->reserved = 0;
}

void __init_idt(void) {
	idt.base = (uint64_t) &idt_entry[0];
	idt.limit = (uint16_t) sizeof(idt_entry) - 1;

	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);

	__set_gate(0, __isr_0);
	__set_gate(1, __isr_1);
	__set_gate(2, __isr_2);
	__set_gate(3, __isr_3);
	__set_gate(4, __isr_4);
	__set_gate(5, __isr_5);
	__set_gate(6, __isr_6);
	__set_gate(7, __isr_7);
	__set_gate(8, __isr_8);
	__set_gate(9, __isr_9);
	__set_gate(10, __isr_10);
	__set_gate(11, __isr_11);
	__set_gate(12, __isr_12);
	__set_gate(13, __isr_13);
	__set_gate(14, __isr_14);
	__set_gate(15, __isr_15);
	__set_gate(16, __isr_16);
	__set_gate(17, __isr_17);
	__set_gate(18, __isr_18);
	__set_gate(19, __isr_19);
	__set_gate(20, __isr_20);
	__set_gate(21, __isr_21);
	__set_gate(22, __isr_22);
	__set_gate(23, __isr_23);
	__set_gate(24, __isr_24);
	__set_gate(25, __isr_25);
	__set_gate(26, __isr_26);
	__set_gate(27, __isr_27);
	__set_gate(28, __isr_28);
	__set_gate(29, __isr_29);
	__set_gate(30, __isr_30);
	__set_gate(31, __isr_31);

	__set_gate(32, __irq_0);
	__set_gate(33, __irq_1);
	__set_gate(34, __irq_2);
	__set_gate(35, __irq_3);
	__set_gate(36, __irq_4);
	__set_gate(37, __irq_5);
	__set_gate(38, __irq_6);
	__set_gate(39, __irq_7);
	__set_gate(40, __irq_8);
	__set_gate(41, __irq_9);
	__set_gate(42, __irq_10);
	__set_gate(43, __irq_11);
	__set_gate(44, __irq_12);
	__set_gate(45, __irq_13);
	__set_gate(46, __irq_14);
	__set_gate(47, __irq_15);

	__register_handler(32, tick);
	__register_handler(33, keyboard_call);

	{ /* Timer */
		outb(0x43, 0x36);
		outb(0x40, (uint8_t) (1193180 / 100 & 0xFF));
		outb(0x40, (uint8_t) ((1193180 / 100 >> 8) & 0xFF));
	}

	__asm__ __volatile__("lidt %0"::"m"(idt));
}

static const char *restrict const exception_messages[] = {
		"division by zero",
		"debug",
		"non maskable interrupt",
		"breakpoint",
		"into detected overflow",
		"out of bounds",
		"invalid opcode",
		"no coprocessor",

		"double fault",
		"coprocessor segment overrun",
		"bad TSS",
		"segment not present",
		"stack fault",
		"general protection Fault",
		"page fault",
		"unknown interrupt",

		"coprocessor fault",
		"alignment check",
		"machine check",
		"reserved",
		"reserved",
		"reserved",
		"reserved",
		"reserved",

		"reserved",
		"reserved",
		"reserved",
		"reserved",
		"reserved",
		"reserved",
		"reserved",
		"reserved"};

void __exception_handler(struct regs *r) {
	_error("Error! Received %s exception!", exception_messages[r->int_no]);

	__asm__ __volatile__("cli;hlt");
}

void __register_handler(int num, irq_t fun) {
	if (num >= 32)
		interrupt_handlers[num - 32] = fun;
}

void __irq_handler(struct regs *r) {
	if (r->int_no >= 40) outb(0xA0, 0x20);
	outb(0x20, 0x20);

	/* TODO: Complete the irqs */
}

__attribute__((unused)) void __interrupt_handler(struct regs *r) {
	if (r->int_no < 32)
		__exception_handler(r);
	else
		__irq_handler(r);
}
