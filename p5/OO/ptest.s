	.data
.true:	.asciiz	"true"
.false:	.asciiz	"false"
.nl:	.asciiz	"\n"
	.text
	.globl  main


j.f:
	#####
	####### PROLOGUE #######
	#####
	subu	$sp, $sp, 12	# ra, fp & locals
	sw	$ra, 8($sp)
	sw	$fp, 4($sp)
	addu	$fp, $sp, 16	# set new fp
#assignment
	li	$t0, 3
	sw	$t0, -16($fp)
#assignment
	lw	$t0, 0($fp)
	lw	$t1, -4($fp)
	add	$t0, $t0, $t1
	lw	$t1, -16($fp)
	add	$t0, $t0, $t1
	sw	$t0, -16($fp)

#return
	lw	$t0, -16($fp)
	move	$v0, $t0
	j	j.f..

j.f..:
	#####
	####### EPILOGUE #######
	#####
	lw	$ra, -8($fp)	# restore ra
	lw	$fp, -12($fp)	# restore fp
	addu	$sp, $sp, 20	# Pop
	jr	$ra

t.f:
	#####
	####### PROLOGUE #######
	#####
	subu	$sp, $sp, 8	# registers ra & fp
	sw	$ra, 4($sp)
	sw	$fp, 0($sp)
	addu	$fp, $sp, 16	# set new fp

t.f..:
	#####
	####### EPILOGUE #######
	#####
	lw	$ra, -12($fp)	# restore ra
	lw	$fp, -16($fp)	# restore fp
	addu	$sp, $sp, 20	# Pop
	jr	$ra

main:
	#####
	####### PROLOGUE #######
	#####
	subu	$sp, $sp, 28	# ra, fp & locals
	sw	$ra, 24($sp)
	sw	$fp, 20($sp)
	addu	$fp, $sp, 24	# set new fp
#assignment
	lw	$t0, -24($fp)
	li	$t1, 3
	li	$t2, 3
	mulo	$t1, $t1, $t2
	add	$t0, $t0, $t1
	sw	$t0, -24($fp)

#return
	li	$t0, 1
	move	$v0, $t0
	j	main..

main..:
	#####
	####### EPILOGUE #######
	#####
	lw	$ra, 0($fp)	# restore ra
	lw	$fp, -4($fp)	# restore fp
	addu	$sp, $sp, 28	# Pop

	li	$v0, 10
	syscall			# program exit
