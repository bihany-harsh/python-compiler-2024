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
	subq $32, %rsp
	movq 16(%rbp), %rdx
	addq $0, %rdx
	movq %rdx, -64(%rbp)
	movq -64(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -72(%rbp)
	movq $3, -72(%rbp)
	movq -72(%rbp), %rax
	movq -64(%rbp), %rdx
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
	subq $48, %rsp
	movq 16(%rbp), %rdx
	addq $0, %rdx
	movq %rdx, -64(%rbp)
	movq -64(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -72(%rbp)
	movq -72(%rbp), %rdx
	addq 24(%rbp), %rdx
	movq %rdx, -80(%rbp)
	movq -80(%rbp), %rdx
	movq %rdx, -88(%rbp)
	movq -88(%rbp), %rax
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
	addq $0, %rdx
	movq %rdx, -64(%rbp)
	movq -64(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -72(%rbp)
	movq $61, -72(%rbp)
	movq -72(%rbp), %rax
	movq -64(%rbp), %rdx
	movq %rax, (%rdx)
	movq 16(%rbp), %rdx
	addq $0, %rdx
	movq %rdx, -80(%rbp)
	movq -80(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -88(%rbp)
	movq -88(%rbp), %rax
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
	movq -64(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -72(%rbp)
	movq $3, -72(%rbp)
	movq -72(%rbp), %rax
	movq -64(%rbp), %rdx
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
	subq $40, %rsp
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq $8
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
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -64(%rbp)
	call func4
	addq $8, %rsp
	movq %rax, -72(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -72(%rbp), %rdx
	movq %rdx, -64(%rbp)
	movq -64(%rbp), %rax
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
	subq $232, %rsp
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq $8
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
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -64(%rbp)
	call func1
	addq $8, %rsp
	movq %rax, -72(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -72(%rbp), %rdx
	movq %rdx, -64(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq $8
	call allocmem
	addq $8, %rsp
	movq %rax, -88(%rbp)
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
	call func5
	movq %rax, -96(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -96(%rbp), %rdx
	movq %rdx, -88(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq $16
	call allocmem
	addq $8, %rsp
	movq %rax, -112(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq $0, %rdx
	imulq $8, %rdx
	movq %rdx, -120(%rbp)
	movq -112(%rbp), %rdx
	addq -120(%rbp), %rdx
	movq %rdx, -128(%rbp)
	movq -64(%rbp), %rax
	movq -128(%rbp), %rdx
	movq %rax, (%rdx)
	movq $1, %rdx
	imulq $8, %rdx
	movq %rdx, -136(%rbp)
	movq -112(%rbp), %rdx
	addq -136(%rbp), %rdx
	movq %rdx, -144(%rbp)
	movq -88(%rbp), %rax
	movq -144(%rbp), %rdx
	movq %rax, (%rdx)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq $1
	movq $0, %rdx
	imulq $8, %rdx
	movq %rdx, -152(%rbp)
	movq -112(%rbp), %rdx
	addq -152(%rbp), %rdx
	movq %rdx, -160(%rbp)
	movq -160(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -168(%rbp)
	pushq -168(%rbp)
	call func2
	addq $16, %rsp
	movq %rax, -176(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -176(%rbp), %rdx
	movq %rdx, -192(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -192(%rbp)
	call print
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq $1, %rdx
	imulq $8, %rdx
	movq %rdx, -200(%rbp)
	movq -112(%rbp), %rdx
	addq -200(%rbp), %rdx
	movq %rdx, -208(%rbp)
	movq -208(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -216(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -216(%rbp)
	call func3
	addq $8, %rsp
	movq %rax, -224(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq $1, %rdx
	imulq $8, %rdx
	movq %rdx, -240(%rbp)
	movq -112(%rbp), %rdx
	addq -240(%rbp), %rdx
	movq %rdx, -248(%rbp)
	movq -248(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -256(%rbp)
	movq -256(%rbp), %rdx
	addq $0, %rdx
	movq %rdx, -264(%rbp)
	movq -264(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -272(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -272(%rbp)
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
