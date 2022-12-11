section .text
bits 64
extern main
global _start
_start:
xchg bx,bx
    call main

    jmp $

global call_detect
call_detect:
    push bx
    mov di, 0x500
    xor bx, bx
    mov edx, 0x534D4150
    mov rax, 0xE820
    int 0x15
    pop bx
    ret