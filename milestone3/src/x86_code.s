	.data
	integer_format: .asciz, "%ld\n"
	.global main
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
	subq $128, %rsp
L6:
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
	movq %rdx, -64(%rbp)
	movq -64(%rbp), %rdx
	cmpq $0, %rdx
	je L29
	movq 40(%rbp), %rax
	cqto
	movq $2, %rbx
	idivq %rbx
	movq %rax, %rdx
	movq %rdx, -72(%rbp)
	movq 32(%rbp), %rdx
	addq -72(%rbp), %rdx
	movq %rdx, -80(%rbp)
	movq -80(%rbp), %rdx
	movq %rdx, -88(%rbp)
	movq -88(%rbp), %rdx
	imulq $0, %rdx
	movq %rdx, -96(%rbp)
	movq 16(%rbp), %rdx
	addq -96(%rbp), %rdx
	movq %rdx, -104(%rbp)
	movq -104(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -112(%rbp)
	movq -112(%rbp), %rdx
	movq 24(%rbp), %rcx
	cmpq %rdx, %rcx
	je 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -120(%rbp)
	movq -120(%rbp), %rdx
	cmpq $0, %rdx
	je L18
	movq -88(%rbp), %rax
	addq $128, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rsi
	popq %rdi
	popq %rbx
	popq %rbp
	ret
	jmp L28
L18:
	movq -88(%rbp), %rdx
	imulq $0, %rdx
	movq %rdx, -128(%rbp)
	movq 16(%rbp), %rdx
	addq -128(%rbp), %rdx
	movq %rdx, -136(%rbp)
	movq -136(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -144(%rbp)
	movq -144(%rbp), %rdx
	movq 24(%rbp), %rcx
	cmpq %rdx, %rcx
	jg 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -152(%rbp)
	movq -152(%rbp), %rdx
	cmpq $0, %rdx
	je L26
	movq -88(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -160(%rbp)
	movq -160(%rbp), %rdx
	movq %rdx, 32(%rbp)
	jmp L28
L26:
	movq -88(%rbp), %rdx
	subq $1, %rdx
	movq %rdx, -168(%rbp)
	movq -168(%rbp), %rdx
	movq %rdx, 40(%rbp)
L28:
	jmp L6
L29:
	xor %rdx, %rdx
	subq $1, %rdx
	movq %rdx, -176(%rbp)
	movq -176(%rbp), %rax
	addq $128, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rsi
	popq %rdi
	popq %rbx
	popq %rbp
	ret
	addq $128, %rsp
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
	subq $40, %rsp
	movq 16(%rbp), %rdx
	movq $1, %rcx
	cmpq %rdx, %rcx
	je 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -64(%rbp)
	movq -64(%rbp), %rdx
	cmpq $0, %rdx
	je L38
	movq $1, %rax
	addq $40, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rsi
	popq %rdi
	popq %rbx
	popq %rbp
	ret
	jmp L45
L38:
	movq 16(%rbp), %rdx
	subq $1, %rdx
	movq %rdx, -72(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -72(%rbp)
	call func2
	addq $8, %rsp
	movq %rax, -80(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq 16(%rbp), %rdx
	imulq -80(%rbp), %rdx
	movq %rdx, -88(%rbp)
	movq -88(%rbp), %rax
	addq $40, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rsi
	popq %rdi
	popq %rbx
	popq %rbp
	ret
L45:
	addq $40, %rsp
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
	subq $184, %rsp
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
	xor %rdx, %rdx
	subq $1, %rdx
	movq %rdx, -192(%rbp)
	movq -200(%rbp), %rdx
	movq -192(%rbp), %rcx
	cmpq %rdx, %rcx
	jne 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -208(%rbp)
	movq -208(%rbp), %rdx
	cmpq $0, %rdx
	je L82
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
	jmp L84
L82:
	xor %rdx, %rdx
	subq $1, %rdx
	movq %rdx, -216(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -216(%rbp)
	call print
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
L84:
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq $5
	call func2
	addq $8, %rsp
	movq %rax, -224(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -224(%rbp), %rdx
	movq %rdx, -232(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -232(%rbp)
	call print
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
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
