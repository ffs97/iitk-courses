.data
	inputString: .space 100
	myMessage: .asciiz "Input a string: "
	
.text
	main:
		li $v0, 4
		li $a0, myMessage
		syscall

		#Storing the input string in inputString variable
		li $v0, 8
		li $a1, 100
		la $a0, inputString
		syscall
		
		#Initialize index to 0
		move $t0, $zero
		loop:
			#Load characted at t0 index into register t1
			lb $t1, inputString($t0)
			
			#Check if it is the end of the string
			beq $t1, 0, exit
			
			#Skip loop if character is not lower case
			blt $t1, 'a', not_lower_case
			bgt $t1, 'z', not_lower_case
			
			#Change lowercase letter to uppercase and store the character at it's index
			addi $t1, $t1, -32
			sb $t1, inputString($t0)
			
			#Continue the loop and add one to the index
			addi $t0, $t0, 1
			j loop
			
		not_lower_case:
			#Skip commands if character is not lower case
			addi $t0, $t0, 1
			j loop
			
		exit:
			# Print the case changed string
			li $v0, 4
			la $a0, inputString
			syscall
			
			#Terminate Code
			li $v0, 10
			syscall
