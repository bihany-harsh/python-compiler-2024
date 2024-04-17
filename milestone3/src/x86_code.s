	.data
	integer_format: .asciz, "%ld\n"
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
	je L30
	movq 32(%rbp), %rdx
	addq 40(%rbp), %rdx
	movq %rdx, -72(%rbp)
	movq -72(%rbp), %rax
	cqto
	movq $2, %rbx
	idivq %rbx
	movq %rax, %rdx
	movq %rdx, -80(%rbp)
	movq -80(%rbp), %rdx
	movq %rdx, -88(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -88(%rbp)
	call print
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -88(%rbp), %rdx
	imulq $8, %rdx
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
	je L19
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
	jmp L29
L19:
	movq -88(%rbp), %rdx
	imulq $8, %rdx
	movq %rdx, -128(%rbp)
	movq 16(%rbp), %rdx
	addq -128(%rbp), %rdx
	movq %rdx, -136(%rbp)
	movq -136(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -144(%rbp)
	movq -144(%rbp), %rdx
	movq 24(%rbp), %rcx
	cmpq %rcx, %rdx
	jl 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -152(%rbp)
	movq -152(%rbp), %rdx
	cmpq $0, %rdx
	je L27
	movq -88(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -160(%rbp)
	movq -160(%rbp), %rdx
	movq %rdx, 32(%rbp)
	jmp L29
L27:
	movq -88(%rbp), %rdx
	subq $1, %rdx
	movq %rdx, -168(%rbp)
	movq -168(%rbp), %rdx
	movq %rdx, 40(%rbp)
L29:
	jmp L6
L30:
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
	subq $248, %rsp
	movq $7, -64(%rbp)
	movq $0, -72(%rbp)
L37:
	movq -72(%rbp), %rdx
	movq -64(%rbp), %rcx
	cmpq %rdx, %rcx
	jg 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -80(%rbp)
	movq -80(%rbp), %rdx
	cmpq $0, %rdx
	je L79
	movq $0, -88(%rbp)
	movq -64(%rbp), %rdx
	subq -72(%rbp), %rdx
	movq %rdx, -96(%rbp)
	movq -96(%rbp), %rdx
	subq $1, %rdx
	movq %rdx, -104(%rbp)
	movq $0, -112(%rbp)
L43:
	movq -112(%rbp), %rdx
	movq -104(%rbp), %rcx
	cmpq %rdx, %rcx
	jg 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -120(%rbp)
	movq -120(%rbp), %rdx
	cmpq $0, %rdx
	je L73
	movq -112(%rbp), %rdx
	imulq $8, %rdx
	movq %rdx, -128(%rbp)
	movq 16(%rbp), %rdx
	addq -128(%rbp), %rdx
	movq %rdx, -136(%rbp)
	movq -112(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -144(%rbp)
	movq -144(%rbp), %rdx
	imulq $8, %rdx
	movq %rdx, -152(%rbp)
	movq 16(%rbp), %rdx
	addq -152(%rbp), %rdx
	movq %rdx, -160(%rbp)
	movq -136(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -168(%rbp)
	movq -160(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -176(%rbp)
	movq -168(%rbp), %rdx
	movq -176(%rbp), %rcx
	cmpq %rcx, %rdx
	jl 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -184(%rbp)
	movq -184(%rbp), %rdx
	cmpq $0, %rdx
	je L71
	movq -112(%rbp), %rdx
	imulq $8, %rdx
	movq %rdx, -192(%rbp)
	movq 16(%rbp), %rdx
	addq -192(%rbp), %rdx
	movq %rdx, -200(%rbp)
	movq -200(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -208(%rbp)
	movq -208(%rbp), %rdx
	movq %rdx, -216(%rbp)
	movq -112(%rbp), %rdx
	imulq $8, %rdx
	movq %rdx, -224(%rbp)
	movq 16(%rbp), %rdx
	addq -224(%rbp), %rdx
	movq %rdx, -232(%rbp)
	movq -112(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -240(%rbp)
	movq -240(%rbp), %rdx
	imulq $8, %rdx
	movq %rdx, -248(%rbp)
	movq 16(%rbp), %rdx
	addq -248(%rbp), %rdx
	movq %rdx, -256(%rbp)
	movq -256(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -264(%rbp)
	movq -264(%rbp), %rax
	movq -232(%rbp), %rdx
	movq %rax, (%rdx)
	movq -112(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -272(%rbp)
	movq -272(%rbp), %rdx
	imulq $8, %rdx
	movq %rdx, -280(%rbp)
	movq 16(%rbp), %rdx
	addq -280(%rbp), %rdx
	movq %rdx, -288(%rbp)
	movq -216(%rbp), %rax
	movq -288(%rbp), %rdx
	movq %rax, (%rdx)
	movq $1, -88(%rbp)
	jmp L71
L71:
	movq -112(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -112(%rbp)
	jmp L43
L73:
	movq -88(%rbp), %rdx
	movq $0, %rcx
	cmpq %rdx, %rcx
	je 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -296(%rbp)
	movq -296(%rbp), %rdx
	cmpq $0, %rdx
	je L77
	jmp L79
	jmp L77
L77:
	movq -72(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -72(%rbp)
	jmp L37
L79:
	movq $0, %rax
	addq $248, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rsi
	popq %rdi
	popq %rbx
	popq %rbp
	ret
	addq $248, %rsp
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
	subq $128, %rsp
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
	movq $4, %rax
	movq -80(%rbp), %rdx
	movq %rax, (%rdx)
	movq $1, %rdx
	imulq $8, %rdx
	movq %rdx, -88(%rbp)
	movq -64(%rbp), %rdx
	addq -88(%rbp), %rdx
	movq %rdx, -96(%rbp)
	movq $1, %rax
	movq -96(%rbp), %rdx
	movq %rax, (%rdx)
	movq $2, %rdx
	imulq $8, %rdx
	movq %rdx, -104(%rbp)
	movq -64(%rbp), %rdx
	addq -104(%rbp), %rdx
	movq %rdx, -112(%rbp)
	movq $2, %rax
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
	movq $7, %rax
	movq -160(%rbp), %rdx
	movq %rax, (%rdx)
	movq $6, %rdx
	imulq $8, %rdx
	movq %rdx, -168(%rbp)
	movq -64(%rbp), %rdx
	addq -168(%rbp), %rdx
	movq %rdx, -176(%rbp)
	movq $8, %rax
	movq -176(%rbp), %rdx
	movq %rax, (%rdx)
	movq -64(%rbp), %rax
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
	subq $32, %rsp
	movq 16(%rbp), %rdx
	addq $0, %rdx
	movq %rdx, -64(%rbp)
	movq 24(%rbp), %rax
	movq -64(%rbp), %rdx
	movq %rax, (%rdx)
	movq 16(%rbp), %rdx
	addq $8, %rdx
	movq %rdx, -72(%rbp)
	movq 24(%rbp), %rdx
	imulq 24(%rbp), %rdx
	movq %rdx, -80(%rbp)
	movq -80(%rbp), %rax
	movq -72(%rbp), %rdx
	movq %rax, (%rdx)
	movq 16(%rbp), %rax
	addq $32, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rsi
	popq %rdi
	popq %rbx
	popq %rbp
	ret
func5:
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
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	call func3
	movq %rax, -64(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -64(%rbp), %rdx
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
	movq -80(%rbp), %rdx
	movq %rdx, -1(%rbp)
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
	pushq -72(%rbp)
	call func1
	addq $32, %rsp
	movq %rax, -88(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -88(%rbp), %rdx
	movq %rdx, -96(%rbp)
	movq -96(%rbp), %rax
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
func6:
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
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	call func3
	movq %rax, -64(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -64(%rbp), %rdx
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
	movq -80(%rbp), %rdx
	movq %rdx, -1(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq $7
	pushq $0
	pushq $7
	pushq -72(%rbp)
	call func1
	addq $32, %rsp
	movq %rax, -88(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -88(%rbp), %rdx
	movq %rdx, -96(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -96(%rbp)
	call print
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
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
