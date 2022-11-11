//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef IDT_H
#define IDT_H

void init_idt(void);

void exception_handler(int num, int err);

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

#endif //IDT_H
