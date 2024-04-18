.section .rodata
.LC0:
    .string "%d\n"
str1:
    .string "bihany"
str2:
    .string "harsh"

.text
.globl main
.type main, @function
main:
.LFB0:
    .cfi_startproc
    endbr64
    pushq %rbp
    .cfi_def_cfa_offset 16
    .cfi_offset 6, -16
    movq %rsp, %rbp
    .cfi_def_cfa_register 6
    subq $32, %rsp

    # Load address of the string "harsh"
    leaq str1(%rip), %rdi
    # Load address of the string "bihany"
    leaq str2(%rip), %rsi
    
    # Call strcmp
    call strcmp@PLT
    movl %eax, %esi
    leaq .LC0(%rip), %rdi
    movl $0, %eax
    call printf@PLT
    movl $0, %eax

    leave
    .cfi_def_cfa 7, 8
    ret
    .cfi_endproc
.LFE0:
    .size main, .-main
