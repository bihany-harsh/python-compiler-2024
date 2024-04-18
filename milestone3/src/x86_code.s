	.data
	string_format: .asciz, "%s\n"
	integer_format: .asciz, "%ld\n"
	d_str: .asciz, "abc"
	t31_str: .asciz, ""
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
	subq $48, %rsp
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
	movq 16(%rbp), %rax
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
	subq $96, %rsp
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
	pushq $2
	pushq $1
	pushq -64(%rbp)
	call func1
	addq $24, %rsp
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
	pushq $16
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
	pushq $3
	pushq $2
	pushq -88(%rbp)
	call func1
	addq $24, %rsp
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
	movq %rax, -104(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq $0, %rdx
	imulq $8, %rdx
	movq %rdx, -112(%rbp)
	movq -104(%rbp), %rdx
	addq -112(%rbp), %rdx
	movq %rdx, -120(%rbp)
	movq -64(%rbp), %rax
	movq -120(%rbp), %rdx
	movq %rax, (%rdx)
	movq $1, %rdx
	imulq $8, %rdx
	movq %rdx, -128(%rbp)
	movq -104(%rbp), %rdx
	addq -128(%rbp), %rdx
	movq %rdx, -136(%rbp)
	movq -88(%rbp), %rax
	movq -136(%rbp), %rdx
	movq %rax, (%rdx)
	movq -104(%rbp), %rax
	addq $96, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rsi
	popq %rdi
	popq %rbx
	popq %rbp
	ret
	addq $96, %rsp
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
	subq $264, %rsp
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	call func2
	movq %rax, -64(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -64(%rbp), %rdx
	movq %rdx, -80(%rbp)
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
	movq %rax, -88(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq $1, %rdx
	imulq $8, %rdx
	movq %rdx, -96(%rbp)
	movq -80(%rbp), %rdx
	addq -96(%rbp), %rdx
	movq %rdx, -104(%rbp)
	movq -104(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -112(%rbp)
	movq -112(%rbp), %rdx
	addq $8, %rdx
	movq %rdx, -120(%rbp)
	movq -120(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -128(%rbp)
	movq $0, %rdx
	imulq $8, %rdx
	movq %rdx, -136(%rbp)
	movq -88(%rbp), %rdx
	addq -136(%rbp), %rdx
	movq %rdx, -144(%rbp)
	movq $1, %rax
	movq -144(%rbp), %rdx
	movq %rax, (%rdx)
	movq $1, %rdx
	imulq $8, %rdx
	movq %rdx, -152(%rbp)
	movq -88(%rbp), %rdx
	addq -152(%rbp), %rdx
	movq %rdx, -160(%rbp)
	movq $2, %rax
	movq -160(%rbp), %rdx
	movq %rax, (%rdx)
	movq $2, %rdx
	imulq $8, %rdx
	movq %rdx, -168(%rbp)
	movq -88(%rbp), %rdx
	addq -168(%rbp), %rdx
	movq %rdx, -176(%rbp)
	movq -128(%rbp), %rax
	movq -176(%rbp), %rdx
	movq %rax, (%rdx)
	movq $2, %rdx
	movq $2, %rcx
	movq $1, %rax
	testq %rcx, %rcx
	jz 2f
1:
	imulq %rdx, %rax
	decq %rcx
	jnz 1b
2:
	movq %rax, %rdx
	movq %rdx, -184(%rbp)
	movq $1, %rdx
	movq -184(%rbp), %rcx
	salq %cl, %rdx
	movq %rdx, -192(%rbp)
	movq -192(%rbp), %rdx
	movq %rdx, -200(%rbp)
	movq -200(%rbp), %rdx
	movq %rdx, -208(%rbp)
	leaq d_str(%rip), %rdx
	movq %rdx, -224(%rbp)
	movq -224(%rbp), %rdx
	movq %rdx, -232(%rbp)
	movq $0, -240(%rbp)
L67:
	movq -240(%rbp), %rdx
	movq $10, %rcx
	cmpq %rdx, %rcx
	jg 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -248(%rbp)
	movq -248(%rbp), %rdx
	cmpq $0, %rdx
	je L76
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -240(%rbp)
	call print
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -240(%rbp), %rdx
	movq $5, %rcx
	cmpq %rdx, %rcx
	jl 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -256(%rbp)
	movq -256(%rbp), %rdx
	cmpq $0, %rdx
	je L74
	jmp L76
	jmp L74
L74:
	movq -240(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -240(%rbp)
	jmp L67
L76:
	movq -240(%rbp), %rdx
	movq $0, %rcx
	cmpq %rdx, %rcx
	jl 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -264(%rbp)
	movq -240(%rbp), %rax
	cqto
	movq $2, %rbx
	idivq %rbx
	movq %rdx, -272(%rbp)
	movq -272(%rbp), %rdx
	movq $0, %rcx
	cmpq %rdx, %rcx
	je 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -280(%rbp)
	movq -264(%rbp), %rdx
	and -280(%rbp), %rdx
	movq %rdx, -288(%rbp)
	movq -288(%rbp), %rdx
	cmpq $0, %rdx
	je L84
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -240(%rbp)
	call print
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -240(%rbp), %rdx
	subq $1, %rdx
	movq %rdx, -240(%rbp)
	jmp L76
L84:
	leaq t31_str(%rip), %rdx
	movq %rdx, -304(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -304(%rbp)
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
	pushq -232(%rbp)
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
	addq $264, %rsp
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
	subq $32, %rsp
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
L97:
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
