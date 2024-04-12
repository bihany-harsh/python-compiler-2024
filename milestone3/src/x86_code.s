	.text
	.globl f_int_int
	.type f_int_int, @function
f_int_int:
.LFB0:
	pushq %rbp
	movq %rsp, %rbp
	movq c, %rax
	popq %rbp
	ret
.LFE0:
	.size f_int_int, .-f_int_int
