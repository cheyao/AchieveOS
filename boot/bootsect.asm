org 0x7C00
KERNEL_OFFSET equ 0x10000

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
    xchg bx,bx
    mov [BOOT_DRIVE], dl

    xor ax, ax
    mov es, ax
    mov ss, ax
    mov ds, ax

    mov bp, 0x7C00
    mov sp, bp ; Stack

    mov dl, [BOOT_DRIVE]
    mov ah, 0x42
    mov si, PACKET
    int 0x13

    mov ax, 0x4F02
    mov bx, 0x4117
    int 0x10

    inc ax
    mov cx, 0x0117
    mov di, 0x1000
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

    mov edi, 0x400000 ; Set page table address
    mov cr3, edi

    ; TBL
    mov DWORD [edi], 0x401003

    ; TBL[0]
    mov DWORD [edi+0x1000], 0x402003

    ; TBL[3]
    mov DWORD [edi+0x1000+8*3], 0x404003

    ; TBL[0][0] ; Random - 0x0000000 to 0x1000000 (0 to 16 Mib) map to itself
    mov DWORD [edi+0x2000], 0x000083 ; Point to bootsect
    mov ebx, [0x1028]
    add ebx, 0x83
    mov DWORD [edi+0x2000+8*1], ebx ; Framebuffer
    mov DWORD [edi+0x2000+8*2], 0x200083 ; Second Framebuffer
    mov DWORD [edi+0x2000+8*3], 0x400083 ; Page table
    mov DWORD [edi+0x2000+8*4], 0x600083 ; Page table

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

bits 64
Realm64:
    mov ax, GDT64_DATA
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov rbp, 0x7C00
    mov rsp, rbp

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

    jmp 0x8000

BOOT_DRIVE: db 0

GDT32:
    .Null:
        dq 0
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

times 446 - ($-$$) db 0

dq 0xFFFFFEEEFFFFFE00 ; MBR
dq 0x0002FB1800000001

times 476-($-$$) db 0

align 4
PACKET: db 0x10
        db 0x00 ; Reserved
        dw 64 ; Sectors to read
        dw 0x8000 ; Address
        dw 0x0000 ; Segment
        dq 0x20 ; Starting LBA


times 510 - ($-$$) db 0
dw 0xaa55
