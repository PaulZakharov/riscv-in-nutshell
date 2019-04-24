	.file	"8-queens.c"
	.option nopic
	.text
	.comm	solution_count,4,4
	.align	2
	.globl	process_solution
	.type	process_solution, @function
process_solution:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	sw	a1,-24(s0)
	lui	a5,%hi(solution_count)
	lw	a5,%lo(solution_count)(a5)
	addi	a4,a5,1
	lui	a5,%hi(solution_count)
	sw	a4,%lo(solution_count)(a5)
	nop
	mv	a0,a5
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	process_solution, .-process_solution
	.align	2
	.globl	is_a_solution
	.type	is_a_solution, @function
is_a_solution:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	sw	a1,-24(s0)
	sw	a2,-28(s0)
	lw	a4,-24(s0)
	lw	a5,-28(s0)
	sub	a5,a4,a5
	seqz	a5,a5
	andi	a5,a5,0xff
	mv	a0,a5
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	is_a_solution, .-is_a_solution
	.align	2
	.globl	construct_candidates
	.type	construct_candidates, @function
construct_candidates:
	addi	sp,sp,-64
	sw	s0,60(sp)
	addi	s0,sp,64
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	sw	a2,-44(s0)
	sw	a3,-48(s0)
	sw	a4,-52(s0)
	lw	a5,-52(s0)
	sw	zero,0(a5)
	li	a5,1
	sw	a5,-20(s0)
	j	.L5
.L11:
	li	a5,1
	sw	a5,-28(s0)
	li	a5,1
	sw	a5,-24(s0)
	j	.L6
.L9:
	lw	a4,-40(s0)
	lw	a5,-24(s0)
	sub	a4,a4,a5
	srai	a5,a4,31
	xor	a4,a5,a4
	sub	a4,a4,a5
	lw	a5,-24(s0)
	slli	a5,a5,2
	lw	a3,-36(s0)
	add	a5,a3,a5
	lw	a5,0(a5)
	lw	a3,-20(s0)
	sub	a5,a3,a5
	srai	a3,a5,31
	xor	a5,a3,a5
	sub	a5,a5,a3
	bne	a4,a5,.L7
	sw	zero,-28(s0)
.L7:
	lw	a5,-24(s0)
	slli	a5,a5,2
	lw	a4,-36(s0)
	add	a5,a4,a5
	lw	a5,0(a5)
	lw	a4,-20(s0)
	bne	a4,a5,.L8
	sw	zero,-28(s0)
.L8:
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
.L6:
	lw	a4,-24(s0)
	lw	a5,-40(s0)
	blt	a4,a5,.L9
	lw	a4,-28(s0)
	li	a5,1
	bne	a4,a5,.L10
	lw	a5,-52(s0)
	lw	a5,0(a5)
	slli	a5,a5,2
	lw	a4,-48(s0)
	add	a5,a4,a5
	lw	a4,-20(s0)
	sw	a4,0(a5)
	lw	a5,-52(s0)
	lw	a5,0(a5)
	addi	a4,a5,1
	lw	a5,-52(s0)
	sw	a4,0(a5)
.L10:
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L5:
	lw	a4,-20(s0)
	lw	a5,-44(s0)
	ble	a4,a5,.L11
	nop
	mv	a0,a5
	lw	s0,60(sp)
	addi	sp,sp,64
	jr	ra
	.size	construct_candidates, .-construct_candidates
	.align	2
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-432
	sw	ra,428(sp)
	sw	s0,424(sp)
	addi	s0,sp,432
	li	a5,1
	sw	a5,-20(s0)
	j	.L13
.L14:
	lui	a5,%hi(solution_count)
	sw	zero,%lo(solution_count)(a5)
	addi	a5,s0,-420
	lw	a2,-20(s0)
	li	a1,0
	mv	a0,a5
	call	backtrack
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L13:
	lw	a5,-20(s0)
	blez	a5,.L14
	li	a5,0
	mv	a0,a5
	lw	ra,428(sp)
	lw	s0,424(sp)
	addi	sp,sp,432
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.0"
	.section	.note.GNU-stack,"",@progbits
