	.file	"c1.c"
	.text
	.globl	f
	.type	f, @function
f:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movl	%esi, -24(%rbp)
	movl	%edx, -28(%rbp)
	movl	%ecx, -32(%rbp)
	movl	%r8d, -36(%rbp)
	movl	%r9d, -40(%rbp)
	movl	$10, -16(%rbp)
	movl	-16(%rbp), %eax
	addl	%eax, %eax
	movl	%eax, -12(%rbp)
	movl	-16(%rbp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	movl	%eax, -8(%rbp)
	movl	-16(%rbp), %eax
	sall	$2, %eax
	movl	%eax, -4(%rbp)
	movl	-20(%rbp), %eax
	imull	-16(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	f, .-f
	.section	.rodata
.LC0:
	.string	"abc"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$4, -16(%rbp)
	movl	$5, -12(%rbp)
	movl	-16(%rbp), %eax
	leal	8(%rax), %r8d
	movl	-16(%rbp), %eax
	leal	7(%rax), %edi
	movl	-16(%rbp), %eax
	leal	6(%rax), %r9d
	movl	-16(%rbp), %eax
	leal	5(%rax), %r10d
	movl	-16(%rbp), %eax
	leal	4(%rax), %ecx
	movl	-12(%rbp), %eax
	leal	3(%rax), %edx
	movl	-16(%rbp), %eax
	leal	2(%rax), %esi
	movl	-16(%rbp), %eax
	addl	$1, %eax
	pushq	%r8
	pushq	%rdi
	movl	%r10d, %r8d
	movl	%eax, %edi
	call	f
	addq	$16, %rsp
	leaq	.LC0(%rip), %rax
	movq	%rax, -8(%rbp)
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
