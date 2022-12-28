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
__attribute__((aligned(0x10))) static isr_t idt_entry[256];
__attribute__((aligned(0x10))) static irq_t interrupt_handlers[16] = {tick, keyboard_call, NULL, NULL, NULL, NULL, NULL,
                                                                      NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                                                                      NULL};

void set_gate(uint8_t vector, void (*isr)(void)) {
	isr_t *descriptor = &idt_entry[vector];

	descriptor->isr_low = (uint64_t) isr & 0xFFFF;
	descriptor->cs = 0x10;
	descriptor->ist = 0;
	descriptor->attributes = 0x8E;
	descriptor->isr_mid = ((uint64_t) isr >> 16) & 0xFFFF;
	descriptor->isr_high = ((uint64_t) isr >> 32) & 0xFFFFFFFF;
	descriptor->reserved = 0;
}

static inline void io_wait(void) {
	outb(0x80, 0);
}

void init_idt(void) {
	idt.base = (uint64_t) &idt_entry[0];
	idt.limit = (uint16_t) sizeof(idt_entry) - 1;

	outb(0x20, 0x11);
	io_wait();
	outb(0xA0, 0x11);
	io_wait();
	outb(0x21, 0x20);
	io_wait();
	outb(0xA1, 0x28);
	io_wait();
	outb(0x21, 0x04);
	io_wait();
	outb(0xA1, 0x02);
	io_wait();
	outb(0x21, 0x01);
	io_wait();
	outb(0xA1, 0x01);
	io_wait();
	outb(0x21, 0x0);
	io_wait();
	outb(0xA1, 0x0);
	io_wait();

	set_gate(0, isr_0);
	set_gate(1, isr_1);
	set_gate(2, isr_2);
	set_gate(3, isr_3);
	set_gate(4, isr_4);
	set_gate(5, isr_5);
	set_gate(6, isr_6);
	set_gate(7, isr_7);
	set_gate(8, isr_8);
	set_gate(9, isr_9);
	set_gate(10, isr_10);
	set_gate(11, isr_11);
	set_gate(12, isr_12);
	set_gate(13, isr_13);
	set_gate(14, isr_14);
	set_gate(15, isr_15);
	set_gate(16, isr_16);
	set_gate(17, isr_17);
	set_gate(18, isr_18);
	set_gate(19, isr_19);
	set_gate(20, isr_20);
	set_gate(21, isr_21);
	set_gate(22, isr_22);
	set_gate(23, isr_23);
	set_gate(24, isr_24);
	set_gate(25, isr_25);
	set_gate(26, isr_26);
	set_gate(27, isr_27);
	set_gate(28, isr_28);
	set_gate(29, isr_29);
	set_gate(30, isr_30);
	set_gate(31, isr_31);

	set_gate(32, irq_0);
	set_gate(33, irq_1);
	set_gate(34, irq_2);
	set_gate(35, irq_3);
	set_gate(36, irq_4);
	set_gate(37, irq_5);
	set_gate(38, irq_6);
	set_gate(39, irq_7);
	set_gate(40, irq_8);
	set_gate(41, irq_9);
	set_gate(42, irq_10);
	set_gate(43, irq_11);
	set_gate(44, irq_12);
	set_gate(45, irq_13);
	set_gate(46, irq_14);
	set_gate(47, irq_15);

	/* Timer */
	outb(0x43, 0x36);
	outb(0x40, (uint8_t) (1193180 / 100 & 0xFF));
	outb(0x40, (uint8_t) ((1193180 / 100 >> 8) & 0xFF));

	__asm__ __volatile__("lidt %0;"
	                     "sti;"
			::"m"(idt));
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

void exception_handler(struct regs *r) {
	error("Error! Received %s exception from address %#lx!", exception_messages[r->int_no], r->rip);

	__asm__ __volatile__("cli;hlt");
}

void register_handler(int num, irq_t fun) {
	if (num >= 32)
		interrupt_handlers[num - 32] = fun;
}

void irq_handler(struct regs *r) {
	if (r->int_no == 39)
		return; // Sus irq

	if (r->int_no >= 40) outb(0xA0, 0x20);
	outb(0x20, 0x20);

	irq_t handler = interrupt_handlers[r->int_no - 32];
	if (handler != NULL)
		handler(r);
}

void interrupt_handler(struct regs *r) {
	if (r->int_no < 32)
		exception_handler(r);
	else if (r->int_no < 48)
		irq_handler(r);
	else if (r->int_no != 39)
		printf("Warning: unhandled int %#lx\n", r->int_no);
}
