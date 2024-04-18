	.data
	string_format: .asciz, "%s\n"
	integer_format: .asciz, "%ld\n"
	a_str: .asciz, "harsh"
	b_str: .asciz, "bihany"
	t2_str: .asciz, "wowo"
	t3_str: .asciz, "no"
	.globl main
	.text
main:
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
	subq $88, %rsp
	leaq a_str(%rip), %rdx
	movq %rdx, -72(%rbp)
	leaq b_str(%rip), %rdx
	movq %rdx, -88(%rbp)
	movq -96(%rbp), %rdx
	cmpq $0, %rdx
	je L9
	leaq t2_str(%rip), %rdx
	movq %rdx, -112(%rbp)
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -112(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
	jmp L11
	leaq t3_str(%rip), %rdx
	movq %rdx, -128(%rbp)
L9:
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq -128(%rbp)
	call printstr
	addq $8, %rsp
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rax
L11:
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
    jz is_print_aligned_str

    pushq $0                 # align to 16 bytes

    ; lea  integer_format(%rip), %rdi
    movq  16(%rbp), %rdi
    movq  $1, %rsi
    xor %rax, %rax          
    call printf@PLT

    add $8, %rsp
    jmp print_done_str

is_print_aligned_str:

    lea  string_format(%rip), %rdi
    movq  16(%rbp), %rsi          
    xor %rax, %rax         
    call printf@PLT
    
print_done_str: 

    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rsi
    popq %rdi
    popq %rbx
    popq %rbp

    ret
