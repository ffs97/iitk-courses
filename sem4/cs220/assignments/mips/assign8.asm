.data
	matrix1: .word 0:20000
	matrix2: .word 0:20000
	answerMatrix: .word 0:20000
	promptMessage1m: .asciiz "Input number of rows in matrix 1: "
	promptMessage1n: .asciiz "Input number of columns in matrix 1: "
	promptMessage1a: .asciiz "Input N*M numbers for matrix 1:\n"
	promptMessage2m: .asciiz "Input number of rows in matrix 2: "
	promptMessage2n: .asciiz "Input number of columns in matrix 2: "
	promptMessage2a: .asciiz "Input N*M numbers for matrix 2:\n"
	newLine: .asciiz "\n"
	tab: .asciiz "\t"
	errorMessage1: .asciiz "Error: No. of columns of first matrix should be eual to no. of rows of second matrix"
	errorMessage2: .asciiz "Error: Matrix Dimensions should be <= 100"
	
.text
	main:
		la $a0, promptMessage1m
		jal printString
		
		jal inputInteger
		move $s0, $v1 #Store no of rows in matrix1 in $s0

		la $a0, errorMessage2
		bge $s0, 100, printError #Print error if dimension bigger than 100
		
		la $a0, promptMessage1n
		jal printString
		
		jal inputInteger
		move $s1, $v1 #Store no of cols in matrix1 in $s1

		la $a0, errorMessage2
		bge $s1, 100, printError #Print error if dimension bigger than 100
		
		la $a0, promptMessage2m
		jal printString
		
		jal inputInteger
		move $s2, $v1 #Store no of rows in matrix2 in $s2
		
		la $a0, errorMessage1
		bne $s2, $s1, printError #Check if number of cols in matrix1 are not equal to nunmber of rows in matrix2, otherwise show error
		
		la $a0, promptMessage2n
		jal printString
		
		jal inputInteger
		move $s2, $v1 #Store no of cols in matrix2 in $s2

		la $a0, errorMessage2
		bge $s2, 100, printError #Print error if dimension bigger than 100
		
		la $a0, promptMessage1a
		jal printString
	
		move $a0, $s0
		move $a1, $s1
		la $a2, matrix1
		jal inputMatrix #Take matrix input for matrix 1
		
		la $a0, promptMessage2a
		jal printString
	
		move $a0, $s1
		move $a1, $s2
		la $a2, matrix2
		jal inputMatrix #Take matrix input for matrix 2
		
		la $s3, answerMatrix
		la $s4, matrix1
		la $s5, matrix2
		
		li $t0, 0
		loop_i: #Loop for row in answer Matrix
			beq $t0, $s0 exit_i
			
			li $t1, 0
			loop_j: #Loop for column in answer Matrix
				beq $t1, $s2, exit_j
				
				mul $t4, $t0, $s2
				add $t4, $t4, $t1
				mul $t4, $t4, 8
				add $t4, $t4, $s3
				l.d $f12, ($t4)
				
				li $t2, 0
				loop_k: #Loop for calculation of (i, j) element of answer matrix
					beq $t2, $s1, exit_k
					
					mul $t3, $t0, $s1
					add $t3, $t3, $t2
					mul $t3, $t3, 8
					add $t3, $t3, $s4
					l.d $f0, ($t3)
					
					mul $t3, $t2, $s2
					add $t3, $t3, $t1
					mul $t3, $t3, 8
					add $t3, $t3, $s5
					l.d $f2, ($t3)
					
					mul.d $f0, $f0, $f2
					add.d $f12, $f12, $f0
					
					add $t2, $t2, 1
					j loop_k
				exit_k:
					s.d $f12, ($t4)
					
					add $t1, $t1, 1
					j loop_j
			exit_j:
				add $t0, $t0, 1
				j loop_i
		exit_i:
			move $a0, $s0
			move $a1, $s2
			move $a2, $s3
			jal printMatrix
		
		j exit
	
	inputMatrix: #Function to take matrix input
		add $sp, $sp, -8
		sw $t4, 0($sp)
		sw $t5, 4($sp)
		
		move $t4, $a2
		mul $t5, $a0, $a1
		mul $t5, $t5, 8
		add $t5, $t4, $t5
		read_loop:
			beq $t4, $t5, exit_read
			
			li $v0, 7
			syscall
			s.d $f0, ($t4)
			
			add $t4, $t4, 8
			j read_loop
			
		exit_read:
			lw $t4, 0($sp)
			lw $t5, 4($sp)
			add $sp, $sp, 8
			
			jr $ra
	
	printMatrix: #Function to Take Matrix Input
		add $sp, $sp, -20
		sw $t4, 0($sp)
		sw $t5, 4($sp)
		sw $t6, 8($sp)
		sw $t7, 12($sp)
		s.d $f12, 16($sp) #May be 20($sp) in QtSpim
		
		move $t7, $a0
		
		move $t4, $a2
		mul $t5, $a0, $a1
		mul $t5, $t5, 8
		add $t5, $t4, $t5
		
		li $t6, 0
		print_loop:
			beq $t4, $t5, exit_print
			
			li $v0, 3
			l.d $f12, ($t4)
			syscall
			
			li $v0, 4
			la $a0, tab
			syscall
			
			add $t4, $t4, 8
			add $t6, $t6, 1
			beq $t6, $a1, print_new_line
			
			j print_loop
			
		print_new_line:
			li $v0, 4
			la $a0, newLine
			syscall
			
			li $t6, 0
			j print_loop
			
		exit_print:
			move $a0, $t7
			
			lw $t4, 0($sp)
			lw $t5, 4($sp)
			lw $t6, 8($sp)
			lw $t7, 12($sp)
			l.d $f12, 16($sp)
			add $sp, $sp, 20
			
			jr $ra
	
	
	printString: #Function to print String
		li $v0, 4
		syscall
		
		jr $ra
		
	inputInteger: #Function to take matrix input
		li $v0, 5
		syscall
		move $v1, $v0
		
		jr $ra
		
	printError: #Function to printError and then end
		li $v0, 4
		syscall

		j exit
		
	exit:
		li $v0, 10
		syscall
