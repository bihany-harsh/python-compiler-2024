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
	movl	%edi, -36(%rbp)
	movl	%esi, -40(%rbp)
	movl	%edx, -44(%rbp)
	movl	%ecx, -48(%rbp)
	movl	%r8d, -52(%rbp)
	movl	%r9d, -56(%rbp)
	movl	$46, -20(%rbp)
	movl	-36(%rbp), %edx
	movl	-40(%rbp), %eax
	addl	%eax, %edx
	movl	-44(%rbp), %eax
	addl	%eax, %edx
	movl	-48(%rbp), %eax
	addl	%eax, %edx
	movl	-52(%rbp), %eax
	addl	%eax, %edx
	movl	-56(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, -16(%rbp)
	movl	-52(%rbp), %eax
	leal	4(%rax), %edx
	movl	-56(%rbp), %eax
	addl	$2, %eax
	imull	%eax, %edx
	movl	16(%rbp), %eax
	imull	-48(%rbp), %eax
	addl	%eax, %edx
	movl	-56(%rbp), %eax
	imull	-44(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, -12(%rbp)
	movl	$5, -8(%rbp)
	movl	$9, -4(%rbp)
	movl	-16(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	f, .-f
	.section	.rodata
.LC0:
	.string	"%d\n"
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
	pushq	$67
	pushq	$6
	movl	$4, %r9d
	movl	$3, %r8d
	movl	$2, %ecx
	movl	$1, %edx
	movl	$20, %esi
	movl	$5, %edi
	call	f
	addq	$16, %rsp
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
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
