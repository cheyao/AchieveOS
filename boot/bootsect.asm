org 0x7C00
KERNEL_OFFSET equ 0x8000
SECTORS equ 120

GDT32_DATA equ GDT32.Data - GDT32
GDT32_CODE equ GDT32.Code - GDT32
GDT64_DATA equ GDT64.Data - GDT64
GDT64_CODE equ GDT64.Code - GDT64

PRESENT        equ 1 << 7
NOT_SYS        equ 1 << 4
EXEC           equ 1 << 3
DC             equ 1 << 2
RW             equ 1 << 1
ACCESSED       equ 1 << 0

GRAN_4K       equ 1 << 7
SZ_32         equ 1 << 6
LONG_MODE     equ 1 << 5

bits 16

    jmp 0:_start

_start:
    mov bp, 0x7BFF
    mov sp, bp ; Stack

    mov ah, 0x0e
    mov al, 'D'
    int 0x10

    jmp $

    mov bx, KERNEL_OFFSET
    mov ah, 0x02
    mov al, SECTORS
    mov cl, 0x02
    mov ch, 0x00
    mov dh, 0x00
    int 0x13

    mov ax, 0x4F02
    mov bx, 0x4117
    int 0x10

    mov ax, 0x4F01
    mov cx, 0x0117
    mov di, 0x7E00
    int 0x10

    in al, 0x92 ; enable a20
    or al, 2
    out 0x92, al

    cli ; 32 bit!
    lgdt [GDT32.Pointer]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp GDT32_CODE:init_pm

bits 32
init_pm:
    mov ax, GDT32_DATA
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov edi, 0x600000 ; Set page table address
    mov cr3, edi

    ; PML4
    mov DWORD [edi], 0x601003

    ; PML3[0]
    mov DWORD [edi+0x1000], 0x602003

    ; PML2[0]
    mov DWORD [edi+0x2000], 0x000083 ; Point to bootsect
    mov ebx, [0x7E28]
    add ebx, 0x83
    mov DWORD [edi+0x2000+8*1], ebx ; Framebuffer
    mov DWORD [edi+0x2000+8*2], 0x200083 ; Second Framebuffer

    mov ebx, 0x200083
    mov edi, 0x602000+8*2
    mov ecx, 32+94
.kernel:
    mov DWORD [edi], ebx
    add ebx, 0x200000
    add edi, 8
    loop .kernel

    ; Enable paging
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr4
    or eax, ((1 << 5) + (1 << 9) + (1 << 10))
    mov cr4, eax

    mov eax, cr0
    or eax, ((1 << 31) + (1 << 1))
    and eax, ~(1 << 2)
    mov cr0, eax

    lgdt [GDT64.Pointer]
    jmp GDT64_CODE:Realm64

    jmp $

bits 64
Realm64:
    mov ax, GDT32_DATA
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov rbp, 0xC000000
    mov rsp, rbp

    jmp KERNEL_OFFSET

BOOT_DRIVE: db 0
ADDRESS: dd 0
DISK_ERROR: db "Disk error!", 0
VBE_ERROR: db "Error: No VBE support!", 0

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

GDT64:
    .Null:
        dq 0
        dq 0
    .Code:
        dd 0xFFFF
        db 0
        db PRESENT | NOT_SYS | EXEC | RW
        db GRAN_4K | LONG_MODE | 0xF
        db 0
    .Data:
        dd 0xFFFF
        db 0
        db PRESENT | NOT_SYS | RW
        db GRAN_4K | SZ_32 | 0xF
        db 0
    .Pointer:
        dw GDT64.Pointer - GDT64 - 1
        dq GDT64

times 510 - ($-$$) db 0
dw 0xaa55
