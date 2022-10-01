[org 0x1000]
[bits 32]
FRAMEBUFFER equ 0x7FFF0

mov eax, [FRAMEBUFFER]
mov byte [eax], 6

jmp $
