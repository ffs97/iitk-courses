.data
	myMessage1: .asciiz "Enter the First Number: "
	myMessage2: .asciiz "Enter the Second Number: "
	
.text
	main:
		li $v0, 4
		la $a0, myMessage1
		syscall
		
		#Read the First Number and save in $a0 as argument 1
		li $v0, 5
		syscall
		move $t0, $v0
		
		li $v0, 4
		la $a0, myMessage2
		syscall
		
		#Read the Second Number and save in $a0 as argument 2
		li $v0, 5
		syscall
		move $t1, $v0
		
		#Call the function (and save location of program counter) to add two numbers and print the return value
		move $a0, $t0
		move $a1, $t1
		jal func
		li $v0, 1
		move $a0, $v1
		syscall
		
		#Terminate the program
		li $v0, 10
		syscall
		
	func:
		#Save the current values of registers in stack which will be used in the function
		addi $sp, $sp, -12
		sw $t4, 0($sp)
		sw $t5, 4($sp)
		sw $t6, 8($sp)
		
		#Save 5*x and 7*y in registers
		mul $t5, $a0, 5
		mul $t6, $a1, 7
		
		#Subtract 5*x and 7*y from zero
		sub $t4, $zero, $t5
		sub $t4, $t4, $t6
		
		#Load 35 and -35 to $t5 and $t6 respectively
		li $t5, 35
		li $t6, -35
		
		#Branching for the if condition
		bgt $t4, $t5, highCondition
		blt $t4, $t6, lowCondition
		
		#Save return value in branches and then jump over other conditions
		move $v1, $t4
		j rest
		
		highCondition:
			li $v1, 35
			j rest
		
		lowCondition:
			li $v1, -35
			j rest
		
		rest:
			#Load the previous values of registers from stack
			lw $t4, 0($sp)
			lw $t5, 4($sp)
			lw $t6, 8($sp)
			addi $sp, $sp, 12
		
			#Jump back to the point where function was called
			jr $ra
