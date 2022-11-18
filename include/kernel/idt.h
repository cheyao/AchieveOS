//
// Created by cheyao on 11.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef IDT_H
#define IDT_H

void init_idt(void);
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
extern void isr_normal_0(void);
extern void isr_normal_1(void);
extern void isr_normal_2(void);
extern void isr_normal_3(void);
extern void isr_normal_4(void);
extern void isr_normal_5(void);
extern void isr_normal_6(void);
extern void isr_normal_7(void);
extern void isr_normal_8(void);
extern void isr_normal_9(void);
extern void isr_normal_10(void);
extern void isr_normal_11(void);
extern void isr_normal_12(void);
extern void isr_normal_13(void);
extern void isr_normal_14(void);
extern void isr_normal_15(void);

typedef void (*isr_t)(void);

void register_handler(int num, isr_t fun);

#endif //IDT_H
