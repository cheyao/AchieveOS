org 0x7C00
KERNEL_OFFSET equ 0x7E00
SECTORS equ 28

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

PRESENT        equ 1 << 7
NOT_SYS        equ 1 << 4
EXEC           equ 1 << 3
DC             equ 1 << 2
RW             equ 1 << 1
ACCESSED       equ 1 << 0

; Flags bits
GRAN_4K       equ 1 << 7
SZ_32         equ 1 << 6
LONG_MODE     equ 1 << 5

bits 16

    mov [BOOT_DRIVE], dl ; Save the boot disk

    mov bp, 0x7BFF
    mov sp, bp ; Stack

    mov ah, 0x01
    mov ch, 0x00
    mov cl, 0x15 ; Set up cursor :)
    int 0x10

    mov bx, KERNEL_OFFSET
    mov dl, [BOOT_DRIVE] ; Read from disk

    mov ah, 0x02
    mov al, SECTORS 
    mov cl, 0x02
    mov ch, 0x00
    mov dh, 0x00
    int 0x13

    jc .disk_error  ; check for errors

    cmp al, SECTORS
    jne .disk_error

    in al, 0x92 ; enable a20
    test al, 2
    jnz .after
    or al, 2
    and al, 0xFE
    out 0x92, al
.after:

    cli ; 32 bit!
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:init_pm

.vbe_error:
    mov dx, VBE_ERROR
    call print

    jmp $

.disk_error:
.sectors_error:
    mov dx, DISK_ERROR
    call print

    jmp $

print:
    cmp byte [bx], 0
    je .return

    mov ah, 0x0e
    mov al, [bx]
    int 0x10

    add bx, 1

    jmp print

.return:
    ret

gdt_start: ; null
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xffff    ; segment limit, bits 0-15
    dw 0x0000    ; segment base, bits 0-15
    db 0x00      ; segment base, bits 16-23
    db 10011010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + limit, bits 16-19
    db 0x00      ; segment base, bits 24-31

gdt_data:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_descriptor:
    dw gdt_descriptor - gdt_start - 1
    dd gdt_start

[bits 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x7FFFF
    mov esp, ebp

    mov edi, 0x1000    ; Set the destination index to 0x1000.
    mov cr3, edi       ; Set control register 3 to the destination index.
    xor eax, eax       ; Nullify the A-register.
    mov ecx, 4096      ; Set the C-register to 4096.
    rep stosd          ; Clear the memory.
    mov edi, cr3       ; Set the destination index to control register 3.

    mov DWORD [edi], 0x2003      ; Set the uint32_t at the destination index to 0x2003.
    add edi, 0x1000              ; Add 0x1000 to the destination index.
    mov DWORD [edi], 0x3003      ; Set the uint32_t at the destination index to 0x3003.
    add edi, 0x1000              ; Add 0x1000 to the destination index.
    mov DWORD [edi], 0x4003      ; Set the uint32_t at the destination index to 0x4003.
    add edi, 0x1000              ; Add 0x1000 to the destination index.

    mov ebx, 0x00000003          ; Set the B-register to 0x00000003.
    mov ecx, 512                 ; Set the C-register to 512. loop counter

.SetEntry:
    mov DWORD [edi], ebx         ; Set the uint32_t at the destination index to the B-register.
    add ebx, 0x1000              ; Add 0x1000 to the B-register.
    add edi, 8                   ; Add eight to the destination index.
    loop .SetEntry               ; Set the next entry.

    mov eax, cr4                 ; Set the A-register to control register 4.
    or eax, 1 << 5               ; Set the PAE-bit, which is the 6th bit (bit 5).
    mov cr4, eax                 ; Set control register 4 to the A-register.

    mov ecx, 0xC0000080          ; Set the C-register to 0xC0000080, which is the EFER MSR.
    rdmsr                        ; Read from the model-specific register.
    or eax, 1 << 8               ; Set the LM-bit which is the 9th bit (bit 8).
    wrmsr                        ; Write to the model-specific register.

    mov eax, cr0                 ; Set the A-register to control register 0.
    or eax, 1 << 31              ; Set the PG-bit, which is the 32nd bit (bit 31).
    mov cr0, eax                 ; Set control register 0 to the A-register. ; Somehow bugs??

    ; Into 32-bit compatibility submode!

    lgdt [GDT64.Pointer]         ; Load the 64-bit global descriptor table.
    jmp GDT_CODE:Realm64       ; Set the code segment and enter 64-bit long mode.

    jmp $

BOOT_DRIVE: db 0
ADDRESS: dd 0
DISK_ERROR: db "Disk error!", 0
VBE_ERROR: db "Error: No VBE support!", 0

GDT64:
    .Null:
        dq 0
        dq 0
    .Code:
        dd 0xFFFF                                   ; Limit & Base (low, bits 0-15)
        db 0                                        ; Base (mid, bits 16-23)
        db PRESENT | NOT_SYS | EXEC | RW            ; Access
        db GRAN_4K | LONG_MODE | 0xF                ; Flags & Limit (high, bits 16-19)
        db 0                                        ; Base (high, bits 24-31)
    .Data:
        dd 0xFFFF                                   ; Limit & Base (low, bits 0-15)
        db 0                                        ; Base (mid, bits 16-23)
        db PRESENT | NOT_SYS | RW                   ; Access
        db GRAN_4K | SZ_32 | 0xF                    ; Flags & Limit (high, bits 16-19)
        db 0                                        ; Base (high, bits 24-31)
    .Pointer:
        dw GDT64.Pointer - GDT64 - 1
        dq GDT64

GDT_DATA equ GDT64.Data - GDT64
GDT_CODE equ GDT64.Code - GDT64

[BITS 64]

Realm64:
    cli                           ; Clear the interrupt flag.

    mov ax, GDT_DATA              ; Set the A-register to the data descriptor.
    mov ds, ax                    ; Set the data segment to the A-register.
    mov es, ax                    ; Set the extra segment to the A-register.
    mov fs, ax                    ; Set the F-segment to the A-register.
    mov gs, ax                    ; Set the G-segment to the A-register.
    mov ss, ax                    ; Set the stack segment to the A-register.

    call KERNEL_OFFSET

    jmp $

times 510 - ($-$$) db 0
dw 0xaa55
