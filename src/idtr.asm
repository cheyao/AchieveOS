bits 64
extern exception_handler

%macro isr_err_stub 1
global isr_%1
isr_%1:
    push %1
    call exception_handler
    iretq
%endmacro

%macro isr_no_err_stub 1
global isr_%1
isr_%1:
    iretq
    push rax
    push %1
    push 0
    call exception_handler
    pop rax
    pop rax
    pop rax
    iretq
%endmacro

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

global flush_idt
flush_idt:
    mov eax, [esp-8]
    lidt [eax]
    ret