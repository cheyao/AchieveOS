section .start
bits 32
extern main
global _start
_start:
    call main
    xchg bx, bx

    jmp $
