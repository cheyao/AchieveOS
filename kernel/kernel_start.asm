section .text
bits 64
extern main
global _start
_start:
    call main
.hlt:
    hlt
    jmp .hlt ; Prevents ints from skipping the hlt
align 16
