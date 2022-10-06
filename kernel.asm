FRAMEBUFFER equ 0xFD000000
WIDTH equ 640
HEIGHT equ 400
NULL equ 0
PASS equ 0xFFFF

org 0x1000
bits 32

; Macro section

%macro entry 1
    dw %1
    dw 0x08
    db 0
    db 0x8E
    dw (%1 - $$ + 0x1000) >> 16
%endmacro

%macro outb 2
    %ifn %1 = PASS
        mov dx, %1
    %endif
    %ifn %2 = PASS
        mov al, %2
    %endif
    out dx, al
%endmacro

%imacro zero 1
    xor %1, %1
%endmacro

; Code section

main:
    outb 0x20, 0x11
    outb 0xA0, PASS
    outb 0x21, 0x20
    outb 0xA1, 0x28
    outb 0x21, 0x04
    outb 0xA1, 0x02
    outb 0x21, 0x01
    outb 0xA1, PASS
    zero al
    outb 0x21, PASS
    outb 0xA1, PASS

    lidt [idtr]
    sti

    outb 0x43, 0x36
    outb 0x40, 174
    outb 0x40,  77

main_loop:

    jmp main_loop


tick:
    outb 0x20, 0x20
    mov eax, [ticks]
    add eax, 1
    mov [ticks], eax
    and eax, 1000b
    cmp eax, 1000b
    jne end
    xor byte [FRAMEBUFFER], 6
    xor byte [FRAMEBUFFER + 1], 6
    xor byte [FRAMEBUFFER + WIDTH], 6
    xor byte [FRAMEBUFFER + WIDTH + 1], 6
end:
    iret

error_isr:
    add esp, 4
no_error_isr:
    iret

; Data section

ticks: dd 0
state: db 0 ; 0 .. menu
            ; 1 .. sc
	    ; 2 ..
	    ; 3 .. 

cursor: db 1 ; 1 .. start game
             ; 2 .. fix 512 bug

fix_bug: db 0

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

