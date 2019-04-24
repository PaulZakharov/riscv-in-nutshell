	.file	"backtrack.c"
	.option nopic
	.text
	.globl	finished
	.section	.sbss,"aw",@nobits
	.align	2
	.type	finished, @object
	.size	finished, 4
finished:
	.zero	4
	.text
	.align	2
	.globl	backtrack
	.type	backtrack, @function
backtrack:
	addi	sp,sp,-448
	sw	ra,444(sp)
	sw	s0,440(sp)
	addi	s0,sp,448
	sw	a0,-436(s0)
	sw	a1,-440(s0)
	sw	a2,-444(s0)
	lw	a2,-444(s0)
	lw	a1,-440(s0)
	lw	a0,-436(s0)
	call	is_a_solution
	mv	a5,a0
	beqz	a5,.L2
	lw	a2,-444(s0)
	lw	a1,-440(s0)
	lw	a0,-436(s0)
	call	process_solution
	j	.L6
.L2:
	lw	a5,-440(s0)
	addi	a5,a5,1
	sw	a5,-440(s0)
	addi	a4,s0,-424
	addi	a5,s0,-420
	mv	a3,a5
	lw	a2,-444(s0)
	lw	a1,-440(s0)
	lw	a0,-436(s0)
	call	construct_candidates
	sw	zero,-20(s0)
	j	.L4
.L7:
	lw	a5,-440(s0)
	slli	a5,a5,2
	lw	a4,-436(s0)
	add	a4,a4,a5
	lw	a5,-20(s0)
	slli	a5,a5,2
	addi	a3,s0,-16
	add	a5,a3,a5
	lw	a5,-404(a5)
	sw	a5,0(a4)
	lw	a2,-444(s0)
	lw	a1,-440(s0)
	lw	a0,-436(s0)
	call	backtrack
	lui	a5,%hi(finished)
	lw	a5,%lo(finished)(a5)
	bnez	a5,.L8
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L4:
	lw	a5,-424(s0)
	lw	a4,-20(s0)
	blt	a4,a5,.L7
	j	.L6
.L8:
	nop
.L6:
	mv	a0,a5
	lw	ra,444(sp)
	lw	s0,440(sp)
	addi	sp,sp,448
	jr	ra
	.size	backtrack, .-backtrack
	.ident	"GCC: (GNU) 8.2.0"
	.section	.note.GNU-stack,"",@progbits
