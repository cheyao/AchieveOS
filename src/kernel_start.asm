FRAMEBUFFER equ 0xFD000000
WIDTH equ 640
HEIGHT equ 400
NULL equ 0
PASS equ 0xFFFF

bits 64
[extern mainC]
[extern tickC]
global main
section text
main:
    mov rdi, 0xB8000
    mov rax, 0x0F200F200F200F20 ; Clear screen :)
    mov rcx, 500
    rep stosq

    call mainC

    jmp $