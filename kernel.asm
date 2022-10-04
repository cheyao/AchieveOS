FRAMEBUFFER equ 0x7FFF0
WIDTH equ 640
HEIGHT equ 400

org 0x1000
bits 32

main:
    mov eax, idtr
    lidt [eax]
    int 0x0
    jmp $

idtr:
    dw idt_end - idt - 1
    dd idt

%macro idt_noerr 0
    dw no_error_isr
    dw 0x08
    db 0
    db 0x8E
    dw (no_error_isr - $$ + 0x1000) >> 16
%endmacro

%macro idt_err 0
    dw error_isr
    dw 0x08
    db 0
    db 0x8E
    dw (error_isr - $$ + 0x1000) >> 16
%endmacro

align 8
idt:
    idt_noerr
    idt_noerr
    idt_noerr
idt_end:

error_isr:
    add esp, 4
no_error_isr:
    iret
