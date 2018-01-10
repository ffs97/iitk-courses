.data
	binaryString: .asciiz "00000000000000000000000000000000"
	myMessage1: .asciiz "Please input an integer (n): "
	myMessage2: .asciiz "Input integer in Binary: "
	newLine: .asciiz "\n"
	
.text
	main:
		#Take input and store in $s0
			li $v0, 4
			la $a0, myMessage1
			syscall
		
			li $v0, 5
			syscall
			move $s0, $v0
			
		#Print message before function call
			li $v0, 4
			la $a0, myMessage2
			syscall
			
		#Call function and print generated string	
			move $a0, $s0
			la $a1, binaryString
			
			jal convertToBinary
		
			li $v0, 4
			la $a0, binaryString
			syscall
				
		j exit
	
	convertToBinary: #Function to convert integer stored in $a0 to binary string stored in $a1
		#Save registers. to be used, in the stack to avoid memory loss
			addi $sp, $sp, -16
			sw $t4, 0($sp)
			sw $t5, 4($sp)
			sw $t6, 8($sp)
			sw $s0, 12($sp)
		
		move $t5, $a0
		move $s0, $a1 #Store address of string to $s0
		addi $s0, $s0, 31 #Make $s0 the last index of string
		
		li $t4, 1 #This will help in getting the least significant bit in the following loop
		loop: #loop to form binary and store in string
			and $t6, $t5, $t4 #Get the least significant bit of $t5 ans store in $t6
			addi $t6, $t6, 48 #Add 48 to convert 0 or 1 into their ascii so as to store them as bytes
			
			sb $t6, ($s0) #Store $t6 in the current index in the given string
			
			srl $t5, $t5, 1
			
			beq $t5, $zero, return #If $t5 is now 0, we have captured all bits and thus we can now end the function
			
			addi $s0, $s0, -1 #Decrease index of string
			j loop
		
		return:
			#Load all previous values of registers to avoid memory loss
				lw $t4, 0($sp)
				lw $t5, 4($sp)
				lw $t6, 8($sp)
				lw $s0, 12($sp)
				addi $sp, $sp, 16
				
			jr $ra
			
	exit:
		li $v0, 10
		syscall
