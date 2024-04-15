	.text
	.section .rodata
.str0:
	.string "%ld\n"
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
	movq -56(%rbp), -16(%rbp)
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
	movq $3, %rax
	addq $4, %rax
	addq $5, %rdx
	leaq (%rdx), %rax
	movq $4, %rdx
	addq $5, %rdx
	movq $6, %rdx
	addq $5, %rdx
	movq $1, %rdi
	movq $0, %rax
	call printf@PLT
	movq %rax, -8(%rbp)
	movq -8(%rbp), %rdi
	call puts@PLT
	movq $1, -16(%rbp)
	movq -16(%rbp), %rsi
	leaq .str0(%rip), %rdi
	movq $0, %rax
	call printf@PLT
	movq $1, %rdi
	movq $0, %rax
	call printf@PLT
	popq %rbp
	ret
.LFE1:
	.size main, .-main
	movq 0(%rbp), %rdx
