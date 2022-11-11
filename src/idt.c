//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include "idt.h"
#include "types.h"
#include "stdio.h"

typedef struct {
    uint16_t isr_low;      // The lower 16 bits of the ISR's address
    uint16_t cs;           // The GDT segment selector that the CPU will load into CS before calling the ISR
    uint8_t ist;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
    uint8_t attributes;   // Type and attributes; see the IDT page
    uint16_t isr_mid;      // The higher 16 bits of the lower 32 bits of the ISR's address
    uint32_t isr_high;     // The higher 32 bits of the ISR's address
    uint32_t reserved;     // Set to zero
} __attribute__((packed)) IDTR;

typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) IDTR_T;

__attribute__((aligned(0x10)))
static IDTR idt_entry_t[255];
static IDTR_T idtr_t;

#define trap 0x8F
#define inte 0x8E

extern void flush_idt(u64int idt);

void fidtr(uint8_t vector, void *isr, uint8_t flags) {
    IDTR *descriptor = &idt_entry_t[vector];

    descriptor->isr_low = (uint64_t) isr & 0xFFFF;
    descriptor->cs = 8;
    descriptor->ist = 0;
    descriptor->attributes = flags;
    descriptor->isr_mid = ((uint64_t) isr >> 16) & 0xFFFF;
    descriptor->isr_high = ((uint64_t) isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved = 0;
}

void init_idt(void) {
    idtr_t.base = (u64int) &idt_entry_t[0];
    idtr_t.limit = (uint16_t) sizeof(IDTR) * 2 - 1;

    fidtr(0, isr_0, inte);
    fidtr(1, isr_1, inte);
    fidtr(2, isr_2, inte);
    fidtr(3, isr_3, inte);
    fidtr(4, isr_4, inte);
    fidtr(5, isr_5, inte);

    flush_idt((u64int) &idtr_t);
}

void exception_handler(int num, int err) {

}
