section .text
bits 64
extern main
global _start
_start:
    xchg bx,bx
    call main

    jmp $
