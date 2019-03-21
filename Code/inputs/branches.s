.section .text
.globl _start
_start:
	addi sp, sp, -32
	li s1, 576839729
	auipc s2, 2
	li s2, 6
loop1:
	addi s2, s2, 1
	li s3, 9
	blt s2, s3, loop1
	beq s2, s3, brnch1
	nop
	nop
brnch1:
	li s2, 11
loop2:
	addi s2, s2, -1
	bge s2, s3, loop2
	bne s2, s3, brnch2
	nop
	nop
brnch2:
	li s2, -1
	li s3, 2
	bltu s3, s2, brnch3
	nop
brnch3:
	bgeu s2, s3, brnch4
	nop
brnch4:
	addi sp, sp, 32
	li s2, 12
	jal s1, jmp1
	li s2, 14
jmp1:
	auipc s1, 0
	jalr s2, s1, 12
	nop
	nop
	li s2, 2
	addi sp, sp, 32
