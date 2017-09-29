.data
	sortedArr: .word 0:100
	myMessage1: .asciiz "Input the length of the array (N): "
	myMessage2: .asciiz "Input N integers (ascending order): "
	myMessage3: .asciiz "Input the search key: "
	newLine: .asciiz "\n"
		
.text
	main:
		la $a0, myMessage1
		jal printString
		
		jal inputInteger
		move $s0, $v1 #Store length of array in $s0
		
		la $a0, myMessage2
		jal printString
		
		move $a0, $s0
		la $a1, sortedArr
		jal readIntoArray #Get array inputs into 'sortedArr'
		
		la $a0, myMessage3
		jal printString
		
		jal inputInteger
		move $s1, $v1 #Store search query in $s1
		
		li $a0, 0
		add $a1, $s0, -1
		la $a2, sortedArr
		move $a3, $s1
	
		jal searchArray #Get the index of search query and store in $v1
		
		li $v0, 1
		move $a0, $v1
		syscall #Print the answer (0 if not found, index starting from 1)
	
		j exit
		
	searchArray: #Function to search for a query in Array
		blt $a1, $a0, return
	
		add $sp, $sp, -20
		sw $t4, 0($sp)
		sw $t5, 4($sp)
		sw $a0, 8($sp)
		sw $a1, 12($sp)
		sw $ra, 16($sp) #Store in stack to avoid memory loss
		
		add $t4, $a0, $a1
		div $t4, $t4, 2
		
		mul $t5, $t4, 4
		add $t5, $t5, $a2
		lw $t5, ($t5)
		
		beq $t5, $a3, indexFound
		
		beq $a1, $a0, return
		
		blt $t5, $a3, goToRight
		bgt $t5, $a3, goToLeft
		
		goToLeft: #Branch if current middle is bigger than query
			add $a1, $t4, -1
			jal searchArray
			
			j loadFromStack
			
		goToRight: #Branch if current middle is smaller than query
			add $a0, $t4, 1
			jal searchArray
			
			j loadFromStack
		
		indexFound: #Branch if current middele is equal to query
			add $v1, $t4, 1
			
			mul $t5, $t4, 4
			add $t5, $t5, -4
			add $t5, $t5, $a2
			lw $t5, ($t5)
			
			bne $t5, $a3, loadFromStack
			add $t4, $t4, -1
			j indexFound
			
		
		loadFromStack: #Load from stack
			lw $t4, 0($sp)
			lw $t5, 4($sp)
			lw $a0, 8($sp)
			lw $a1, 12($sp)
			lw $ra, 16($sp)
			add $sp, $sp, 20
		
		return:
			jr $ra
			
	
	readIntoArray: #Function to load input into array
		addi $sp, $sp, -8
		sw $t4, 0($sp)
		sw $t5, 4($sp)
		
		mul $t5, $a0, 4
		move $t4, $a1
		add $t5, $t5, $t4
		
		read_loop:
			beq $t4, $t5, exit_read
			
			li $v0, 5
			syscall
			sw $v0, ($t4)
			
			addi $t4, $t4, 4
			
			j read_loop
			
		exit_read:
			lw $t4, 0($sp)
			lw $t5, 4($sp)
			addi $sp, $sp, 8
			
			jr $ra
	
	printString: #Function to print a string
		li $v0, 4
		syscall
		
		jr $ra
		
	inputInteger: #Function to take an integer as input and store in $v1
		li $v0, 5
		syscall
		move $v1, $v0
		
		jr $ra
		
	exit:
		li $v0, 10
		syscall