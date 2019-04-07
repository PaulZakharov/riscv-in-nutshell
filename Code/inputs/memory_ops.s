.section .text
.globl _start
_start:
    addi sp, sp, -32
    li s2, 0xf000f0f0
    sw s2, 28(sp)
    sh s2, 24(sp)
    sb s2, 20(sp)
    lw s1, 28(sp)
    lh s1, 28(sp)
    lb s1, 28(sp)
    lhu s1, 28(sp)
    lbu s1, 28(sp)
    lw s1, 24(sp)
    lw s1, 20(sp)
    addi sp, sp, 32
	nop
	nop
	nop
	nop

