.data
	arr: .word 0:100
	myMessage1: .asciiz "Please input the Array Length (N): "
	myMessage2: .asciiz "Please input the N numbers \n"
	myMessage3: .asciiz "Sorted Array: \n"
	underflowMessage: .asciiz "Underflow Error, 1 <= n <= 100 allowed"
	overflowMessage: .asciiz "Overflow Error, 1 <= n <= 100 allowed"
	newLine: .asciiz "\n"

.text
	main:
		li $v0, 4
		la $a0, myMessage1
		syscall
		
		li $v0, 5
		syscall
		
		move $s0, $v0
		
		li $t0, 0
		ble $s0, $t0, underflow
		
		li $t0, 100
		bgt $s0, $t0, overflow
		
		
		li $v0, 4
		la $a0, myMessage2
		syscall
		
		move $a0, $s0
		jal readIntoArray
		jal sortArray
		
		li $v0, 4
		la $a0, myMessage3
		syscall
		
		move $a0, $s0
		jal printArray
		
		j exit
		
		
		
		
		
	
		readIntoArray:
			addi $sp, $sp, -8
			sw $t4, 0($sp)
			sw $t5, 4($sp)
						
			li $t4, 0
			mul $t5, $a0, 4
			
			read_loop:
				beq $t4, $t5, exit_read
				
				li $v0, 5
				syscall
				sw $v0, arr($t4)
				
				addi $t4, $t4, 4
				
				j read_loop
				
			exit_read:
				lw $t4, 0($sp)
				lw $t5, 4($sp)
				addi $sp, $sp, 8
				
				jr $ra
				
				
				
				
				
		sortArray:
			addi $sp, $sp, -32
			sw $t4, 0($sp)
			sw $t5, 4($sp)
			sw $t6, 8($sp)
			sw $t7, 12($sp)
			sw $t8, 16($sp)
			sw $a0, 20($sp)
			sw $a1, 24($sp)
			sw $ra, 28($sp)
			
			li $t4, 0
			mul $t6, $a0, 4
			
			main_sort_loop:
				beq $t6, $zero, stop_main_loop
				
				li $t4, 0
				addi $t5, $t4, 4
				secondary_sort_loop:
					beq $t5, $t6, stop_secondary_loop
				
					lw $t7, arr($t4)
					lw $t8, arr($t5)
					
					ble $t7, $t8, sort_continue
					
					move $a0, $t4
					move $a1, $t5
					jal swap_numbers
					
					sort_continue:
						move $t4, $t5
						addi $t5, $t4, 4
					
						j secondary_sort_loop
						
					stop_secondary_loop:
						addi $t6, $t6, -4
						j main_sort_loop
				
				stop_main_loop:
					lw $t4, 0($sp)
					lw $t5, 4($sp)
					lw $t6, 8($sp)
					lw $t7, 12($sp)
					lw $t8, 16($sp)
					lw $a0, 20($sp)
					lw $a1, 24($sp)
					lw $ra, 28($sp)
					addi $sp, $sp, 32

					jr $ra
					
					
					
					
					
		swap_numbers:
			addi $sp, $sp, -8
			sw $t4, 0($sp)
			sw $t5, 4($sp)
			
			lw $t4, arr($a0)
			lw $t5, arr($a1)
						
			sw $t5, arr($a0)	
			sw $t4, arr($a1)
					
			lw $t4, 0($sp)
			lw $t5, 4($sp)
			addi $sp, $sp, 8
				
			jr $ra
							
					
		
		printArray:
			addi $sp, $sp, -12
			sw $t4, 0($sp)
			sw $t5, 4($sp)
			sw $a0, 8($sp)
			
			li $t4, 0
			mul $t5, $a0, 4
			
			print_loop:
				beq $t4, $t5, exit_print
				
				li $v0, 1
				lw $a0, arr($t4)
				syscall
				
				li $v0, 4
				la $a0, newLine
				syscall
				
				addi $t4, $t4, 4
				
				j print_loop
				
			exit_print:
				lw $t4, 0($sp)
				lw $t5, 4($sp)
				lw $a0, 8($sp)
				addi $sp, $sp, 12
				
				jr $ra
		
		
		
		
		underflow:
			li $v0, 4
			la $a0, underflowMessage
			syscall
			
			j exit

		overflow:
			li $v0, 4
			la $a0, overflowMessage
			syscall
			
			j exit
					
					
					
					
		exit:
			li $v0, 10
			syscall

