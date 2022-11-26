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

typedef struct {
	uint16_t isr_low;    // The lower 16 bits of the ISR's address
	uint16_t cs;         // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t ist;         // The IST in the TSS that the CPU will load into RSP; set to zero for now
	uint8_t attributes;  // Type and attributes; see the IDT page
	uint16_t isr_mid;    // The higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t isr_high;   // The higher 32 bits of the ISR's address
	uint32_t reserved;   // Set to zero
} __attribute__((packed)) IDTR;

typedef struct {
	uint16_t limit;
	uint64_t base;
} __attribute__((packed)) IDTR_T;

__attribute__((aligned(0x10))) static IDTR idt_entry_t[256];
isr_t interrupt_handlers[224];
static IDTR_T idtr_t;

extern void flush_idt(uint64_t idt);

void fidtr(uint8_t vector, void *isr) {
	for (int i = 0; i < 224; i++)
		interrupt_handlers[i] = 0;

	IDTR *descriptor = &idt_entry_t[vector];

	descriptor->isr_low = (uint64_t) isr & 0xFFFF;
	descriptor->cs = 0x10;
	descriptor->ist = 0;
	descriptor->attributes = 0x8E;
	descriptor->isr_mid = ((uint64_t) isr >> 16) & 0xFFFF;
	descriptor->isr_high = ((uint64_t) isr >> 32) & 0xFFFFFFFF;
	descriptor->reserved = 0;
}

void init_idt(void) {
	idtr_t.base = (uint64_t) &idt_entry_t[0];
	idtr_t.limit = (uint16_t) sizeof(idt_entry_t) - 1;

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

	fidtr(0, isr_0);
	fidtr(1, isr_1);
	fidtr(2, isr_2);
	fidtr(3, isr_3);
	fidtr(4, isr_4);
	fidtr(5, isr_5);
	fidtr(6, isr_6);
	fidtr(7, isr_7);
	fidtr(8, isr_8);
	fidtr(9, isr_9);
	fidtr(10, isr_10);
	fidtr(11, isr_11);
	fidtr(12, isr_12);
	fidtr(13, isr_13);
	fidtr(14, isr_14);
	fidtr(15, isr_15);
	fidtr(16, isr_16);
	fidtr(17, isr_17);
	fidtr(18, isr_18);
	fidtr(19, isr_19);
	fidtr(20, isr_20);
	fidtr(21, isr_21);
	fidtr(22, isr_22);
	fidtr(23, isr_23);
	fidtr(24, isr_24);
	fidtr(25, isr_25);
	fidtr(26, isr_26);
	fidtr(27, isr_27);
	fidtr(28, isr_28);
	fidtr(29, isr_29);
	fidtr(30, isr_30);
	fidtr(31, isr_31);

	fidtr(32, isr_normal_0);
	fidtr(33, isr_normal_1);
	fidtr(34, isr_normal_2);
	fidtr(35, isr_normal_3);
	fidtr(36, isr_normal_4);
	fidtr(37, isr_normal_5);
	fidtr(38, isr_normal_6);
	fidtr(39, isr_normal_7);
	fidtr(40, isr_normal_8);
	fidtr(41, isr_normal_9);
	fidtr(42, isr_normal_10);
	fidtr(43, isr_normal_11);
	fidtr(44, isr_normal_12);
	fidtr(45, isr_normal_13);
	fidtr(46, isr_normal_14);
	fidtr(47, isr_normal_15);

	register_handler(32, tick);
	register_handler(33, keyboard_call);

	{ /* Timer */
		outb(0x43, 0x36);
		outb(0x40, (uint8_t) (1193180 / 100 & 0xFF));
		outb(0x40, (uint8_t) ((1193180 / 100 >> 8) & 0xFF));
	}

	flush_idt((uint64_t) &idtr_t);
}

const char *exception_messages[] = {
		"Division By Zero",
		"Debug",
		"Non Maskable Interrupt",
		"Breakpoint",
		"Into Detected Overflow",
		"Out of Bounds",
		"Invalid Opcode",
		"No Coprocessor",

		"Double Fault",
		"Coprocessor Segment Overrun",
		"Bad TSS",
		"Segment Not Present",
		"Stack Fault",
		"General Protection Fault",
		"Page Fault",
		"Unknown Interrupt",

		"Coprocessor Fault",
		"Alignment Check",
		"Machine Check",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",

		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved"};

void exception_handler(int num, int err) {
	printf("Got interrupt: %s (%d), error: %#x", exception_messages[num], num, err);

	__asm__ __volatile__("hlt");
}

void register_handler(int num, isr_t fun) {
	if (num >= 32)
		interrupt_handlers[num - 32] = fun;
}

void isr_handler(int num) {
	if (num >= 8) outb(0xA0, 0x20); /* Keep this just so I remember the ports*/
	outb(0x20, 0x20);

	if (num == 14 || num == 15);
	else if (interrupt_handlers[num] == 0)
		printf("Received irq %d\n", num);
	else
		interrupt_handlers[num]();
}

// /usr/local/bin/qemu-system-x86_64 -no-reboot -cdrom cdrom.iso --trace events=trace