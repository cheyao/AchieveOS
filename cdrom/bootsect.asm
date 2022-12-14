; Booting from a cdrom
; Its just for copying the stuff onto the disk

org 0x7C00
bits 16

GDT32_DATA equ GDT32.Data - GDT32
GDT32_CODE equ GDT32.Code - GDT32

jmp 0:start
start:
    mov bp, 0x7BFF
    mov sp, bp ; Stack

    mov bx, 0x8000
    mov ah, 0x02
    mov al, 30
    mov cl, 0x02
    mov ch, 0x00
    mov dh, 0x00
    int 0x13

    in al, 0x92 ; enable a20
    or al, 2
    out 0x92, al

    cli ; 32 bit!
    lgdt [GDT32.Pointer]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp GDT32_CODE:pm

bits 32

pm:
    mov ax, GDT32_DATA
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x8000

GDT32:
    .Null:
        dd 0x0
        dd 0x0
    .Code:
        dw 0xffff    ; segment limit, bits 0-15
        dw 0x0000    ; segment base, bits 0-15
        db 0x00      ; segment base, bits 16-23
        db 10011010b ; flags (8 bits)
        db 11001111b ; flags (4 bits) + limit, bits 16-19
        db 0x00      ; segment base, bits 24-31
    .Data:
        dw 0xffff
        dw 0x0000
        db 0x00
        db 10010010b
        db 11001111b
        db 0x00
    .Pointer:
        dw GDT32.Pointer - GDT32 - 1
        dd GDT32

times 510 - ($-$$) db 0
dw 0xaa55