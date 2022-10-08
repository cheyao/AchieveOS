org 0x7C00
KERNEL_OFFSET equ 0x00001000
SECTORS equ 16

    mov [BOOT_DRIVE], dl
    
    mov ah, 0x01
    mov ch, 0x3F
    int 0x10

    mov ax, 0x7000
    mov es, ax
    mov di, 0xF000
    mov ax, 0x4F01
    mov cx, 0x0100
    int 0x10
    xor cx, cx
    mov es, cx

    cmp ax, 0x004F
    je cont

    mov     edx, -30208
    mov     eax, edx
    out dx, ax
    mov     eax, -29984
    out dx, ax

    ; Bochs interrupt

cont:

    mov bp, 0x7BFF
    mov sp, bp

    mov bx, KERNEL_OFFSET
    mov dl, [BOOT_DRIVE]

    mov ah, 0x02
    mov al, SECTORS 
    mov cl, 0x02
    mov ch, 0x00
    mov dh, 0x00

    int 0x13 
    jc disk_error

    cmp al, SECTORS
    jne sectors_error

    mov ax, 0x4F02
    mov bx, 0x4100
    int 0x10

    cli 
    lgdt [gdt_descriptor] 
    mov eax, cr0
    or eax, 0x1 
    mov cr0, eax
    jmp CODE_SEG:init_pm 


disk_error:
sectors_error:
disk_loop:
    mov dx, DISK_ERROR
    call print

    jmp $

print:
    cmp byte [bx], 0
    je return

    mov ah, 0x0e
    mov al, [bx]
    int 0x10

    add bx, 1

    jmp print

return:
    ret

DISK_ERROR: db "Disk error!", 0

gdt_start:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x0

gdt_data:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

[bits 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x20000
    mov esp, ebp

    call KERNEL_OFFSET
    jmp $


BOOT_DRIVE: db 0
ADDRESS: dd 0


times 510 - ($-$$) db 0
dw 0xaa55
