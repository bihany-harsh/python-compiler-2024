	.data
	string_format: .asciz, "%s\n"
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
	subq $200, %rsp
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
	movq $1, %rdx
	imulq $8, %rdx
	movq %rdx, -184(%rbp)
	movq -80(%rbp), %rdx
	addq -184(%rbp), %rdx
	movq %rdx, -192(%rbp)
	movq -192(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -200(%rbp)
	movq -200(%rbp), %rdx
	addq $8, %rdx
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
	call print
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	movq $2, %rdx
	imulq $8, %rdx
	movq %rdx, -224(%rbp)
	movq -88(%rbp), %rdx
	addq -224(%rbp), %rdx
	movq %rdx, -232(%rbp)
	movq -232(%rbp), %rdx
	movq (%rdx), %rdx
	movq %rdx, -240(%rbp)
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
	addq $200, %rsp
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
L79:
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
