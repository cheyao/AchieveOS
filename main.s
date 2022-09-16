	.file	"main.c"
	.text
	.local	idt
	.comm	idt,384,8
	.local	idtr
	.comm	idtr,6,4
	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	movl	$32512, %eax
	movl	(%eax), %eax
	movl	$4660, (%eax)
	call	init_idt
	call	init_keyboard
	nop
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 12.2.0"
