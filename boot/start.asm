section .start
bits 64
extern main
global _start
_start:
    mov ebx, [0x1028]
    add ebx, 0x83
    mov DWORD [0x602000+8*1], ebx ; Framebuffer
    mov DWORD [0x602000+8*2], 0x200083 ; Second Framebuffer
    mov DWORD [0x602000+8*4], 0x600083 ; Page table

    ; TBL[3][0]
    ; Kernel memory - 0x1000000 to 0x8000000 (16 Mib to 128 Mib) map to 0xC0000000 - 0xC8000000
    mov rcx, 56
    mov rdi, 0x604000
    mov rbx, 0x1000083
.kernel:
    mov [rdi], rbx
    add rdi, 8
    add rbx, 0x200000
    loop .kernel

    call main

    jmp rax
