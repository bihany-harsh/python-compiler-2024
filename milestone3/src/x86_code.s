	.data
	string_format: .asciz, "%s\n"
	integer_format: .asciz, "%ld\n"
	t1_str: .asciz, "in bs"
	t9_str: .asciz, "done 2"
	t16_str: .asciz, "done"
	t27_str: .asciz, "hello"
	t43_str: .asciz, "Before starting the function"
	t46_str: .asciz, "Element is present at index:"
	t48_str: .asciz, "this is another string"
	t51_str: .asciz, "After the function call"
	t52_str: .asciz, "hello hehre"
	t56_str: .asciz, "Sum is correct"
	t57_str: .asciz, "Sum is incorrect"
	t58_str: .asciz, "After the check"
	t60_str: .asciz, "inside loop"
	t61_str: .asciz, "here now"
	c_str: .asciz, "harsh"
	b_str: .asciz, "bihany"
	t63_str: .asciz, "wowo"
	t64_str: .asciz, "no"
	.globl main
	.text
func1:
	pushq %rbp
	movq %rsp, %rbp
	pushq %rbx
	pushq %rdi
	pushq %rsi
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	subq $184, %rsp
	leaq t1_str(%rip), %rdx
	movq %rdx, -72(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -72(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
L8:
	movq 32(%rbp), %rdx
	movq 40(%rbp), %rcx
	cmpq %rdx, %rcx
	jge 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -80(%rbp)
	movq -80(%rbp), %rdx
	cmpq $0, %rdx
	je L33
	movq 32(%rbp), %rdx
	addq 40(%rbp), %rdx
	movq %rdx, -88(%rbp)
	movq -88(%rbp), %rax
	cqto
	movq $2, %rbx
	idivq %rbx
	movq %rax, %rdx
	movq %rdx, -96(%rbp)
	movq -96(%rbp), %rdx
	movq %rdx, -104(%rbp)
	movq -104(%rbp), %rdx
	imulq $8, %rdx
	movq %rdx, -112(%rbp)
	movq 16(%rbp), %rdx
	addq -112(%rbp), %rdx
	movq %rdx, -120(%rbp)
	movq -120(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -128(%rbp)
	movq -128(%rbp), %rdx
	movq 24(%rbp), %rcx
	cmpq %rdx, %rcx
	je 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -136(%rbp)
	movq -136(%rbp), %rdx
	cmpq $0, %rdx
	je L22
	leaq t9_str(%rip), %rdx
	movq %rdx, -152(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -152(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -104(%rbp), %rax
	addq $184, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rsi
	popq %rdi
	popq %rbx
	popq %rbp
	ret
	jmp L32
L22:
	movq -104(%rbp), %rdx
	imulq $8, %rdx
	movq %rdx, -160(%rbp)
	movq 16(%rbp), %rdx
	addq -160(%rbp), %rdx
	movq %rdx, -168(%rbp)
	movq -168(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -176(%rbp)
	movq -176(%rbp), %rdx
	movq 24(%rbp), %rcx
	cmpq %rdx, %rcx
	jg 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -184(%rbp)
	movq -184(%rbp), %rdx
	cmpq $0, %rdx
	je L30
	movq -104(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -192(%rbp)
	movq -192(%rbp), %rdx
	movq %rdx, 32(%rbp)
	jmp L32
L30:
	movq -104(%rbp), %rdx
	subq $1, %rdx
	movq %rdx, -200(%rbp)
	movq -200(%rbp), %rdx
	movq %rdx, 40(%rbp)
L32:
	jmp L8
L33:
	leaq t16_str(%rip), %rdx
	movq %rdx, -216(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -216(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	xor %rdx, %rdx
	subq $1, %rdx
	movq %rdx, -224(%rbp)
	movq -224(%rbp), %rax
	addq $184, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rsi
	popq %rdi
	popq %rbx
	popq %rbp
	ret
	addq $184, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rsi
	popq %rdi
	popq %rbx
	popq %rbp
	ret
func2:
	pushq %rbp
	movq %rsp, %rbp
	pushq %rbx
	pushq %rdi
	pushq %rsi
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	subq $72, %rsp
	movq 16(%rbp), %rdx
	addq $0, %rdx
	movq %rdx, -64(%rbp)
	movq -64(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -72(%rbp)
	movq 24(%rbp), %rdx
	movq %rdx, -72(%rbp)
	movq -72(%rbp), %rax
	movq -64(%rbp), %rdx
	movq %rax, (%rdx)
	movq 16(%rbp), %rdx
	addq $8, %rdx
	movq %rdx, -80(%rbp)
	movq -80(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -88(%rbp)
	movq 32(%rbp), %rdx
	movq %rdx, -88(%rbp)
	movq -88(%rbp), %rax
	movq -80(%rbp), %rdx
	movq %rax, (%rdx)
	movq 16(%rbp), %rdx
	addq $16, %rdx
	movq %rdx, -96(%rbp)
	movq -96(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -104(%rbp)
	movq 24(%rbp), %rdx
	addq 32(%rbp), %rdx
	movq %rdx, -112(%rbp)
	movq -112(%rbp), %rdx
	movq %rdx, -104(%rbp)
	movq -104(%rbp), %rax
	movq -96(%rbp), %rdx
	movq %rax, (%rdx)
	movq 16(%rbp), %rax
	addq $72, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rsi
	popq %rdi
	popq %rbx
	popq %rbp
	ret
func3:
	pushq %rbp
	movq %rsp, %rbp
	pushq %rbx
	pushq %rdi
	pushq %rsi
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	subq $48, %rsp
	movq 16(%rbp), %rdx
	addq $16, %rdx
	movq %rdx, -64(%rbp)
	movq -64(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -72(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -72(%rbp)
	call print
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	leaq t27_str(%rip), %rdx
	movq %rdx, -88(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -88(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	addq $48, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rsi
	popq %rdi
	popq %rbx
	popq %rbp
	ret
main:
func4:
	pushq %rbp
	movq %rsp, %rbp
	pushq %rbx
	pushq %rdi
	pushq %rsi
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	subq $496, %rsp
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq $56
	call allocmem
	addq $8, %rsp
	movq %rax, -64(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq $0, %rdx
	imulq $8, %rdx
	movq %rdx, -72(%rbp)
	movq -64(%rbp), %rdx
	addq -72(%rbp), %rdx
	movq %rdx, -80(%rbp)
	movq $3, %rax
	movq -80(%rbp), %rdx
	movq %rax, (%rdx)
	movq $1, %rdx
	imulq $8, %rdx
	movq %rdx, -88(%rbp)
	movq -64(%rbp), %rdx
	addq -88(%rbp), %rdx
	movq %rdx, -96(%rbp)
	movq $4, %rax
	movq -96(%rbp), %rdx
	movq %rax, (%rdx)
	movq $2, %rdx
	imulq $8, %rdx
	movq %rdx, -104(%rbp)
	movq -64(%rbp), %rdx
	addq -104(%rbp), %rdx
	movq %rdx, -112(%rbp)
	movq $5, %rax
	movq -112(%rbp), %rdx
	movq %rax, (%rdx)
	movq $3, %rdx
	imulq $8, %rdx
	movq %rdx, -120(%rbp)
	movq -64(%rbp), %rdx
	addq -120(%rbp), %rdx
	movq %rdx, -128(%rbp)
	movq $6, %rax
	movq -128(%rbp), %rdx
	movq %rax, (%rdx)
	movq $4, %rdx
	imulq $8, %rdx
	movq %rdx, -136(%rbp)
	movq -64(%rbp), %rdx
	addq -136(%rbp), %rdx
	movq %rdx, -144(%rbp)
	movq $7, %rax
	movq -144(%rbp), %rdx
	movq %rax, (%rdx)
	movq $5, %rdx
	imulq $8, %rdx
	movq %rdx, -152(%rbp)
	movq -64(%rbp), %rdx
	addq -152(%rbp), %rdx
	movq %rdx, -160(%rbp)
	movq $8, %rax
	movq -160(%rbp), %rdx
	movq %rax, (%rdx)
	movq $6, %rdx
	imulq $8, %rdx
	movq %rdx, -168(%rbp)
	movq -64(%rbp), %rdx
	addq -168(%rbp), %rdx
	movq %rdx, -176(%rbp)
	movq $9, %rax
	movq -176(%rbp), %rdx
	movq %rax, (%rdx)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq $7
	pushq $0
	pushq $4
	pushq -64(%rbp)
	call func1
	addq $32, %rsp
	movq %rax, -184(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -184(%rbp), %rdx
	movq %rdx, -200(%rbp)
	leaq t43_str(%rip), %rdx
	movq %rdx, -216(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -216(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -200(%rbp)
	call print
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	xor %rdx, %rdx
	subq $1, %rdx
	movq %rdx, -224(%rbp)
	movq -200(%rbp), %rdx
	movq -224(%rbp), %rcx
	cmpq %rdx, %rcx
	jne 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -232(%rbp)
	movq -232(%rbp), %rdx
	cmpq $0, %rdx
	je L105
	leaq t46_str(%rip), %rdx
	movq %rdx, -248(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -248(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -200(%rbp)
	call print
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	jmp L107
L105:
	xor %rdx, %rdx
	subq $1, %rdx
	movq %rdx, -256(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -256(%rbp)
	call print
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
L107:
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq $19999
	call print
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	leaq t48_str(%rip), %rdx
	movq %rdx, -272(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -272(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq $24
	call allocmem
	addq $8, %rsp
	movq %rax, -280(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq $6
	pushq $5
	pushq -280(%rbp)
	call func2
	addq $24, %rsp
	movq %rax, -288(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -288(%rbp), %rdx
	movq %rdx, -280(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -280(%rbp)
	call func3
	addq $8, %rsp
	movq %rax, -304(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	leaq t51_str(%rip), %rdx
	movq %rdx, -328(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -328(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	leaq t52_str(%rip), %rdx
	movq %rdx, -344(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -344(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -280(%rbp), %rdx
	addq $16, %rdx
	movq %rdx, -352(%rbp)
	movq -352(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -360(%rbp)
	movq -360(%rbp), %rdx
	movq $11, %rcx
	cmpq %rdx, %rcx
	je 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -368(%rbp)
	movq -368(%rbp), %rdx
	cmpq $0, %rdx
	je L133
	leaq t56_str(%rip), %rdx
	movq %rdx, -384(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -384(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	jmp L135
L133:
	leaq t57_str(%rip), %rdx
	movq %rdx, -400(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -400(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
L135:
	leaq t58_str(%rip), %rdx
	movq %rdx, -416(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -416(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
L137:
	movq -200(%rbp), %rdx
	movq $0, %rcx
	cmpq %rdx, %rcx
	jl 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -424(%rbp)
	movq -424(%rbp), %rdx
	cmpq $0, %rdx
	je L143
	leaq t60_str(%rip), %rdx
	movq %rdx, -440(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -440(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -200(%rbp), %rdx
	subq $1, %rdx
	movq %rdx, -200(%rbp)
	jmp L137
L143:
	leaq t61_str(%rip), %rdx
	movq %rdx, -456(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -456(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq $10, -464(%rbp)
	leaq c_str(%rip), %rdx
	movq %rdx, -480(%rbp)
	leaq b_str(%rip), %rdx
	movq %rdx, -496(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -496(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	pushq %rbp
	movq %rsp, %rbp
	pushq %rbx
	pushq %rdi
	pushq %rsi
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq -480(%rbp), %rdi
	movq -496(%rbp), %rsi
	call strcmp@PLT
	movq %rax, %rdx
	movq $0, %rcx
	cmpq %rdx, %rcx
	jge 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -504(%rbp)
	movq -504(%rbp), %rdx
	cmpq $0, %rdx
	je L154
	leaq t63_str(%rip), %rdx
	movq %rdx, -520(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -520(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	jmp L156
L154:
	leaq t64_str(%rip), %rdx
	movq %rdx, -536(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -536(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
L156:
	movq $60, %rax
	xor %rdi, %rdi
	syscall
print:
    pushq	%rbp
    mov	%rsp,	%rbp
    pushq	%rbx
    pushq	%rdi
    pushq	%rsi
    pushq	%r12
    pushq	%r13
    pushq	%r14
    pushq	%r15

    testq $15, %rsp
    jz is_print_aligned

    pushq $0                 # align to 16 bytes

    lea  integer_format(%rip), %rdi
    movq  16(%rbp), %rsi
    xor %rax, %rax          
    call printf

    add $8, %rsp
    jmp print_done

is_print_aligned:

    lea  integer_format(%rip), %rdi
    movq  16(%rbp), %rsi          
    xor %rax, %rax         
    call printf
    
print_done: 

    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rsi
    popq %rdi
    popq %rbx
    popq %rbp

    ret
allocmem:
    pushq	%rbp
    mov	%rsp,	%rbp
    pushq	%rbx
    pushq	%rdi
    pushq	%rsi
    pushq	%r12
    pushq	%r13
    pushq	%r14
    pushq	%r15

    testq $15, %rsp
    jz is_mem_aligned

    pushq $0                 # align to 16 bytes
    
    movq 16(%rbp), %rdi
    call malloc

    add $8, %rsp             # remove padding

    jmp mem_done

is_mem_aligned:

    movq 16(%rbp), %rdi
    call malloc
   
mem_done: 

    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rsi
    popq %rdi
    popq %rbx
    popq %rbp

    ret
printstr:
    pushq   %rbp
    mov     %rsp,   %rbp
    pushq   %rbx
    pushq   %rdi
    pushq   %rsi
    pushq   %r12
    pushq   %r13
    pushq   %r14
    pushq   %r15

    testq   $15, %rsp
    jz      is_print_aligned_str

    pushq   $0                 # align to 16 bytes

    lea     string_format(%rip), %rdi   # Load the address of the format string for strings
    movq    16(%rbp), %rsi              # Load the string argument from the caller's frame
    xor     %rax, %rax                  # Clear %rax (since printf uses %al to determine number of XMM arguments)
    call    printf

    add     $8, %rsp
    jmp     print_done_str

is_print_aligned_str:

    lea     string_format(%rip), %rdi   # Load the address of the format string for strings
    movq    16(%rbp), %rsi              # Load the string argument from the caller's frame
    xor     %rax, %rax                  # Clear %rax
    call    printf

print_done_str: 

    popq    %r15
    popq    %r14
    popq    %r13
    popq    %r12
    popq    %rsi
    popq    %rdi
    popq    %rbx
    popq    %rbp

    ret
