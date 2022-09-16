%define SECTORS 16

[org 0x7C00]
KERNEL_OFFSET equ 0x00007E00
PAGING_OFFSET equ 0x00001000

    mov ax, 0x4F02
    mov bx, 0x4100 ; 4000
    int 0x10

    mov di, 0xF000 ; just put the info here
    mov ax, 0x4F01 ;mode
    mov cx, 0x0100 ; video mode number
    int 0x10 ;call

    mov eax, [0xF000 + 0x28] ; get the address of the framebuffer
    mov bx, 0x7000
    mov gs, bx
    mov [gs:0xFFF0], eax
    xor bx, bx
    mov gs, bx

    mov [BOOT_DRIVE], dl
    mov bp, 0x7BFF
    mov sp, bp

    mov bx, KERNEL_OFFSET ; Read from disk and store in 0x1000
    mov dl, [BOOT_DRIVE]

    mov ah, 0x02 ; ah <- int 0x13 function. 0x02 = 'read'
    mov al, SECTORS   ; al <- number of sectors to read (0x01 .. 0x80)
    mov cl, 0x02 ; cl <- sector (0x01 .. 0x11)
                 ; 0x01 is our boot sector, 0x02 is the first 'available' sector
    mov ch, 0x00 ; ch <- cylinder (0x0 .. 0x3FF, upper 2 bits in 'cl')
    ; dl <- drive number. Our caller sets it as a parameter and gets it from BIOS
    ; (0 = floppy, 1 = floppy2, 0x80 = hdd, 0x81 = hdd2)
    mov dh, 0x00 ; dh <- head number (0x0 .. 0xF)

    ; [es:bx] <- pointer to buffer where the data will be stored
    ; caller sets it up for us, and it is actually the standard location for int 13h
    int 0x13      ; BIOS interrupt
    jc disk_error ; if error (stored in the carry bit)

    cmp al, SECTORS    ; BIOS also sets 'al' to the # of sectors read. Compare it.
    jne sectors_error

    cli ; 1. disable interrupts
    lgdt [gdt_descriptor] ; 2. load the GDT descriptor
    mov eax, cr0
    or eax, 0x1 ; 3. set 32-bit mode bit in cr0
    mov cr0, eax
    jmp CODE_SEG:init_pm ; 4. far jump by using a different segment


disk_error:
sectors_error:
disk_loop:
    mov ah, 0x0E

    mov al, 'D'
    int 0x10

    mov al, 'E'
    int 0x10

    jmp $

gdt_start: ; don't remove the labels, they're needed to compute sizes and jumps
    ; the GDT starts with a null 8-byte
    dd 0x0 ; 4 byte
    dd 0x0 ; 4 byte

; GDT for code segment. base = 0x00000000, length = 0xfffff
; for flags, refer to os-dev.pdf document, page 36
gdt_code:
    dw 0xffff    ; segment limit, bits 0-15
    dw 0x0000    ; segment base, bits 0-15
    db 0x00      ; segment base, bits 16-23
    db 10011010b ; flags (8 bits)
.code_limit:
    db 11001111b ; flags (4 bits) + limit, bits 16-19
    db 0x00      ; segment base, bits 24-31

; GDT for data segment. base and length identical to code segment
; some flags changed, again, refer to os-dev.pdf
gdt_data:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; size (16 bit), always one less of its true size
    dd gdt_start               ; address (32 bit)

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

[bits 32]
init_pm: ; we are now using 32-bit instructions
    mov ax, DATA_SEG ; 5. update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000 ; 6. update the stack right at the top of the free space
    mov esp, ebp

    call KERNEL_OFFSET
    jmp $


BOOT_DRIVE: db 0 ; It is a good idea to store it in memory because 'dl' may get overwritten
ADDRESS: dd 0

; padding
times 510 - ($-$$) db 0
dw 0xaa55
