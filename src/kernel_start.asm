FRAMEBUFFER equ 0xFD000000
WIDTH equ 640
HEIGHT equ 400
NULL equ 0
PASS equ 0xFFFF

bits 32
[extern mainC]
[extern tickC]

; Macro section

%macro entry 1
    dw %1
    dw 0x08
    db 0
    db 0x8E
    dw (%1 - $$ + 0x1000) >> 16
%endmacro

%imacro zero 1
    xor %1, %1
%endmacro

; Code section

    mov eax, [FRAMEBUFFER]
    mov byte [eax], 6

    mov     eax, 17
    mov     edx, 32
    out dx, al
    mov     edx, 160
    out dx, al
    mov     ebx, 33
    mov     eax, 32
    mov     edx, ebx
    out dx, al
    mov     ecx, 161
    mov     eax, 40
    mov     edx, ecx
    out dx, al
    mov     eax, 4
    mov     edx, ebx
    out dx, al
    mov     eax, 2
    mov     edx, ecx
    out dx, al
    mov     eax, 1
    mov     edx, ebx
    out dx, al
    mov     edx, ecx
    out dx, al
    xor     eax, eax
    mov     edx, ebx
    out dx, al
    mov     edx, ecx
    out dx, al

    lidt [idtr]
    sti

    mov     eax, 54
    mov     edx, 67
    out dx, al
    mov     edx, 64
    mov     eax, -82
    out dx, al
    mov     eax, 77
    out dx, al

    call mainC

    jmp $

tick:
    push eax
    push edx
    mov     eax, 0x20
    mov     edx, 0x20
    out dx, al
    pop edx
    pop eax
    call tickC
    iret

error_isr:
    add esp, 4
no_error_isr:
    iret

align 16
idtr:
    dw idt_end - idt - 1
    dd idt

idt:
    entry no_error_isr
    entry no_error_isr
    entry no_error_isr
    entry no_error_isr
    entry no_error_isr
    entry no_error_isr
    entry no_error_isr
    entry no_error_isr
    entry error_isr
    entry no_error_isr ; 10
    entry error_isr
    entry error_isr
    entry error_isr
    entry error_isr
    entry error_isr
    entry no_error_isr
    entry no_error_isr
    entry error_isr
    entry no_error_isr
    entry no_error_isr ; 20
    entry no_error_isr
    entry no_error_isr
    entry no_error_isr
    entry no_error_isr
    entry no_error_isr
    entry no_error_isr
    entry no_error_isr
    entry no_error_isr
    entry no_error_isr
    entry no_error_isr ; 30
    entry error_isr
    entry no_error_isr ; 32
    entry tick
idt_end: