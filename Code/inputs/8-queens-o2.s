	.file	"8-queens.c"
	.option nopic
	.text
	.align	2
	.globl	process_solution
	.type	process_solution, @function
process_solution:
	lui	a4,%hi(solution_count)
	lw	a5,%lo(solution_count)(a4)
	addi	a5,a5,1
	sw	a5,%lo(solution_count)(a4)
	ret
	.size	process_solution, .-process_solution
	.align	2
	.globl	is_a_solution
	.type	is_a_solution, @function
is_a_solution:
	sub	a2,a1,a2
	seqz	a0,a2
	ret
	.size	is_a_solution, .-is_a_solution
	.align	2
	.globl	construct_candidates
	.type	construct_candidates, @function
construct_candidates:
	sw	zero,0(a4)
	blez	a2,.L5
	addi	t6,a2,1
	li	t4,1
	li	t5,1
.L6:
	addi	t1,a0,4
	addi	a7,a1,-1
	li	a6,1
	ble	a1,t5,.L12
.L9:
	lw	a2,0(t1)
	addi	t1,t1,4
	sub	a5,t4,a2
	srai	t3,a5,31
	xor	a5,t3,a5
	sub	a5,a5,t3
	sub	a5,a5,a7
	snez	a5,a5
	sub	a2,a2,t4
	neg	a5,a5
	snez	a2,a2
	and	a6,a6,a5
	neg	a2,a2
	addi	a7,a7,-1
	and	a6,a6,a2
	bnez	a7,.L9
	beq	a6,t5,.L12
.L10:
	addi	t4,t4,1
	bne	t4,t6,.L6
.L5:
	ret
.L12:
	lw	a5,0(a4)
	slli	a5,a5,2
	add	a5,a3,a5
	sw	t4,0(a5)
	lw	a5,0(a4)
	addi	a5,a5,1
	sw	a5,0(a4)
	j	.L10
	.size	construct_candidates, .-construct_candidates
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	_start
	.type	_start, @function
_start:
	addi	sp,sp,-416
	sw	s0,408(sp)
	sw	s1,404(sp)
	sw	s2,400(sp)
	sw	ra,412(sp)
	li	s0,1
	lui	s2,%hi(solution_count)
	li	s1,11
.L17:
	mv	a2,s0
	li	a1,0
	addi	s0,s0,1
	mv	a0,sp
	sw	zero,%lo(solution_count)(s2)
	call	backtrack
	bne	s0,s1,.L17
	lw	ra,412(sp)
	lw	s0,408(sp)
	lw	s1,404(sp)
	lw	s2,400(sp)
	li	a0,0
	addi	sp,sp,416
	jr	ra
	.size	_start, .-_start
	.comm	solution_count,4,4
	.ident	"GCC: (GNU) 8.2.0"
	.section	.note.GNU-stack,"",@progbits
