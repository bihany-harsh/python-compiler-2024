	.data
	string_format: .asciz, "%s\n"
	integer_format: .asciz, "%ld\n"
	t1_str: .asciz, "static polymorphism"
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
	subq $24, %rsp
	movq 16(%rbp), %rdx
	movq $2, %rcx
	salq %cl, %rdx
	movq %rdx, -64(%rbp)
	movq -64(%rbp), %rax
	addq $24, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rsi
	popq %rdi
	popq %rbx
	popq %rbp
	ret
	addq $24, %rsp
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
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	call func1
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
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -80(%rbp)
	call print
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
L28:
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
