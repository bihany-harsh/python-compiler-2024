	.text
	.globl c
	.data
	.align 4
	.type c, @object
	.size c, 4
c:
	.long 5
	.globl b
	.type b, @object
	.size b, 1
b:
	.byte 1
	.globl a
	.type a, @object
	.size a, 6
a:
	.string "hello"
