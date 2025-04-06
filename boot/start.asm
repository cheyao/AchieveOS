section .start
bits 64
extern main
global _start
_start:
    mov eax, DWORD [0x1028]
    mov edx, 0x1000083
    mov QWORD [0x602010], 0x200083
    mov QWORD [0x602020], 0x600083 ; Page
    add eax, 0x83
    mov QWORD [0x602008], rax

    mov QWORD [0x602000+5*8], 0xE00083 ; 2 Mib reserved for disk
    xchg bx,bx

    mov     eax, 0x604000
.L2:
    mov     QWORD [rax], rdx ; 0x1000083
    add     rax, 0x08
    add     rdx, 0x200000
    cmp     rax, 0x604000 + 56 * 8
    jne     .L2

    call main

    jmp rax
