	.data
	string_format: .asciz, "%s\n"
	integer_format: .asciz, "%ld\n"
	t56_str: .asciz, "Element is present at index:"
	t57_str: .asciz, "Element is not present"
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
	subq $136, %rsp
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
	je L18
	movq -88(%rbp), %rax
	addq $136, %rsp
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
	addq $136, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rsi
	popq %rdi
	popq %rbx
	popq %rbp
	ret
	addq $136, %rsp
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
	movq $0, -64(%rbp)
L36:
	movq -64(%rbp), %rdx
	movq 24(%rbp), %rcx
	cmpq %rdx, %rcx
	jg 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -72(%rbp)
	movq -72(%rbp), %rdx
	cmpq $0, %rdx
	je L76
	movq 24(%rbp), %rdx
	subq -64(%rbp), %rdx
	movq %rdx, -80(%rbp)
	movq -80(%rbp), %rdx
	subq $1, %rdx
	movq %rdx, -88(%rbp)
	movq $0, -96(%rbp)
L41:
	movq -96(%rbp), %rdx
	movq -88(%rbp), %rcx
	cmpq %rdx, %rcx
	jg 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -104(%rbp)
	movq -104(%rbp), %rdx
	cmpq $0, %rdx
	je L74
	movq -96(%rbp), %rdx
	imulq $8, %rdx
	movq %rdx, -112(%rbp)
	movq 16(%rbp), %rdx
	addq -112(%rbp), %rdx
	movq %rdx, -120(%rbp)
	movq -120(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -128(%rbp)
	movq -96(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -136(%rbp)
	movq -136(%rbp), %rdx
	imulq $8, %rdx
	movq %rdx, -144(%rbp)
	movq 16(%rbp), %rdx
	addq -144(%rbp), %rdx
	movq %rdx, -152(%rbp)
	movq -152(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -160(%rbp)
	movq -128(%rbp), %rdx
	movq -160(%rbp), %rcx
	cmpq %rdx, %rcx
	jl 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -168(%rbp)
	movq -168(%rbp), %rdx
	cmpq $0, %rdx
	je L72
	movq -96(%rbp), %rdx
	imulq $8, %rdx
	movq %rdx, -176(%rbp)
	movq 16(%rbp), %rdx
	addq -176(%rbp), %rdx
	movq %rdx, -184(%rbp)
	movq -184(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -192(%rbp)
	movq -192(%rbp), %rdx
	movq %rdx, -200(%rbp)
	movq -96(%rbp), %rdx
	imulq $8, %rdx
	movq %rdx, -208(%rbp)
	movq 16(%rbp), %rdx
	addq -208(%rbp), %rdx
	movq %rdx, -216(%rbp)
	movq -216(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -224(%rbp)
	movq -96(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -232(%rbp)
	movq -232(%rbp), %rdx
	imulq $8, %rdx
	movq %rdx, -240(%rbp)
	movq 16(%rbp), %rdx
	addq -240(%rbp), %rdx
	movq %rdx, -248(%rbp)
	movq -248(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -256(%rbp)
	movq -256(%rbp), %rdx
	movq %rdx, -224(%rbp)
	movq -224(%rbp), %rax
	movq -216(%rbp), %rdx
	movq %rax, (%rdx)
	movq -96(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -264(%rbp)
	movq -264(%rbp), %rdx
	imulq $8, %rdx
	movq %rdx, -272(%rbp)
	movq 16(%rbp), %rdx
	addq -272(%rbp), %rdx
	movq %rdx, -280(%rbp)
	movq -280(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -288(%rbp)
	movq -200(%rbp), %rdx
	movq %rdx, -288(%rbp)
	movq -288(%rbp), %rax
	movq -280(%rbp), %rdx
	movq %rax, (%rdx)
	jmp L72
L72:
	movq -96(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -96(%rbp)
	jmp L41
L74:
	movq -64(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -64(%rbp)
	jmp L36
L76:
	movq 16(%rbp), %rax
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
	subq $208, %rsp
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq $40
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
	movq $6, %rax
	movq -96(%rbp), %rdx
	movq %rax, (%rdx)
	movq $2, %rdx
	imulq $8, %rdx
	movq %rdx, -104(%rbp)
	movq -64(%rbp), %rdx
	addq -104(%rbp), %rdx
	movq %rdx, -112(%rbp)
	movq $1, %rax
	movq -112(%rbp), %rdx
	movq %rax, (%rdx)
	movq $3, %rdx
	imulq $8, %rdx
	movq %rdx, -120(%rbp)
	movq -64(%rbp), %rdx
	addq -120(%rbp), %rdx
	movq %rdx, -128(%rbp)
	movq $4, %rax
	movq -128(%rbp), %rdx
	movq %rax, (%rdx)
	movq $4, %rdx
	imulq $8, %rdx
	movq %rdx, -136(%rbp)
	movq -64(%rbp), %rdx
	addq -136(%rbp), %rdx
	movq %rdx, -144(%rbp)
	movq $9, %rax
	movq -144(%rbp), %rdx
	movq %rax, (%rdx)
	movq $5, -152(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -152(%rbp)
	pushq -64(%rbp)
	call func2
	addq $16, %rsp
	movq %rax, -160(%rbp)
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq -160(%rbp), %rdx
	movq %rdx, -64(%rbp)
	movq -152(%rbp), %rdx
	subq $1, %rdx
	movq %rdx, -176(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -176(%rbp)
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
	movq %rdx, -208(%rbp)
	movq -200(%rbp), %rdx
	movq -208(%rbp), %rcx
	cmpq %rdx, %rcx
	jne 1f
	movq $0, %rdx
	jmp 2f
1:
	movq $1, %rdx
	jmp 2f
2:
	movq %rdx, -216(%rbp)
	movq -216(%rbp), %rdx
	cmpq $0, %rdx
	je L118
	leaq t56_str(%rip), %rdx
	movq %rdx, -232(%rbp)
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
	jmp L120
L118:
	leaq t57_str(%rip), %rdx
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
L120:
	addq $208, %rsp
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
L129:
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
