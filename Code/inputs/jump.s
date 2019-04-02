.section .text
.globl _start
_start:
	addi sp, sp, -32
    li s2, 5
    nop
    nop
    nop
    jal s1, jump
    nop
    nop
    nop
    nop
    nop
jump: addi s2, s2, 1
    li s1, 6
    add s2, s2, s1