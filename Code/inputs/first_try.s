.section .text
.globl _start
_start:
	li s1, 0x6
	li s2, 0x9
	add s3, s2, s1
	addi sp, sp, -32
	sw s3, 24(sp)
	lw s4, 24(sp)
	slti s1, s4, 0x10
