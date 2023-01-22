section .init

.option norvc

.type start, @function
.global start
start:
	.cfi_startproc

.option push
.option norelax
	la gp, global_pointer
.option pop
	li sp, 0x10000

	la t0, kmain
	csrw mepc, t0

	/* Jump to kernel! */
	tail kmain

	.cfi_endproc

.end
