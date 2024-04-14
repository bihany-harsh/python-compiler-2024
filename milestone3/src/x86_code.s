	.text
	.globl f_int_int_int_int_int_int_int
	.type f_int_int_int_int_int_int_int, @function
f_int_int_int_int_int_int_int:
.LFB0:
	pushq %rbp
	movq %rsp, %rbp
	movq %rdi, -56(%rbp)
	movq %rsi, -64(%rbp)
	movq %rdx, -72(%rbp)
	movq %rcx, -80(%rbp)
	movq %r8, -88(%rbp)
	movq %r9, -96(%rbp)
	movq -56(%rbp), %rdx
	movq -64(%rbp), %rax
	addq %rdx, %rax
	movq -72(%rbp), %rcx
	leaq (%rax), %rdx
	addq %rcx, %rdx
	leaq (%rdx), %rax
	movq %rax, -8(%rbp)
	movq -56(%rbp), %rax
	addq $2, %rax
	movq %rax, -8(%rbp)
	movq $5, -16(%rbp)
	movq %rax, -16(%rbp)
	movq $7, %rax
	addq $8, %rax
	movq %rax, -16(%rbp)
	movq -8(%rbp), %rdx
	movq -16(%rbp), %rax
	addq %rdx, %rax
	popq %rbp
	ret
.LFE0:
	.size f_int_int_int_int_int_int_int, .-f_int_int_int_int_int_int_int
	.globl main
	.type main, @function
main:
.LFB1:
	pushq %rbp
	movq %rsp, %rbp
	popq %rbp
	ret
.LFE1:
	.size main, .-main
	movq 0(%rbp), %rcx
	leaq (%rax), %rdx
	leaq (%rdx), %rax
