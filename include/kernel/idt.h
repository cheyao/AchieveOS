//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef IDT_H
#define IDT_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

void init_idt(void);

void schedule(int port);

extern void isr_0(void);

extern void isr_1(void);

extern void isr_2(void);

extern void isr_3(void);

extern void isr_4(void);
extern void isr_5(void);
extern void isr_6(void);
extern void isr_7(void);
extern void isr_8(void);
extern void isr_9(void);
extern void isr_10(void);
extern void isr_11(void);
extern void isr_12(void);
extern void isr_13(void);
extern void isr_14(void);
extern void isr_15(void);
extern void isr_16(void);
extern void isr_17(void);
extern void isr_18(void);
extern void isr_19(void);
extern void isr_20(void);
extern void isr_21(void);

extern void isr_22(void);

extern void isr_23(void);

extern void isr_24(void);

extern void isr_25(void);

extern void isr_26(void);

extern void isr_27(void);

extern void isr_28(void);

extern void isr_29(void);

extern void isr_30(void);

extern void isr_31(void);

extern void irq_0(void);

extern void irq_1(void);

extern void irq_2(void);

extern void irq_3(void);

extern void irq_4(void);

extern void irq_5(void);

extern void irq_6(void);

extern void irq_7(void);

extern void irq_8(void);

extern void irq_9(void);

extern void irq_10(void);

extern void irq_11(void);

extern void irq_12(void);

extern void irq_13(void);

extern void irq_14(void);

extern void irq_15(void);

typedef void (*irq_t)(void);

void register_handler(int num, irq_t fun);

struct regs {
	/* Registers :) */
	uintptr_t r15, r14, r13, r12, r11, r10, r9, r8, rbp, rdi, rsi, rdx, rcx, rbx, rax;
	/* Error? */
	uintptr_t int_no, err_code;
	/* Registers by int */
	uintptr_t rip, cs, rflags, rsp, ss;
};

typedef struct {
	uint16_t isr_low;    // The lower 16 bits of the ISR's address
	uint16_t cs;         // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t ist;         // The IST in the TSS that the CPU will load into RSP; set to zero for now
	uint8_t attributes;  // Type and attributes; see the IDT page
	uint16_t isr_mid;    // The higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t isr_high;   // The higher 32 bits of the ISR's address
	uint32_t reserved;   // Set to zero
} __attribute__((packed)) isr_t;

typedef struct {
	uint16_t limit;
	uint64_t base;
} __attribute__((packed)) idt_t;

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  // IDT_H
