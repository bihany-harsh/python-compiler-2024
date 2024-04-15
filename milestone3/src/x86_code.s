	.text
	.section .rodata
.str0:
	.string "%ld\n"
	.text
	.globl f_int_int
	.type f_int_int, @function
f_int_int:
.LFB0:
	pushq %rbp
	movq %rsp, %rbp
	movq %rdi, -16(%rbp)
	movq %rsi, -24(%rbp)
	movq -16(%rbp), %rdx
	movq -24(%rbp), %rax
	addq %rdx, %rax
	movq $t1, %rax
	popq %rbp
	ret
.LFE0:
	.size f_int_int, .-f_int_int
	.globl main
	.type main, @function
main:
.LFB1:
	pushq %rbp
	movq %rsp, %rbp
	movq %rax, -8(%rbp)
	subq $16, %rsp
	movq -8(%rbp), %rsi
	leaq .str0(%rip), %rdi
	movq $0, %rax
	call printf@PLT
	movq $0, %rax
	popq %rbp
	ret
.LFE1:
	.size main, .-main
