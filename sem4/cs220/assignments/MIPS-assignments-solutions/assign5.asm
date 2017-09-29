.data
	fibArray: .word 0:20
	myMessage: .asciiz "Please input the number (n): "
	underflowMessage: .asciiz "Underflow Error, 0 <= n <= 20 allowed"
	overflowMessage: .asciiz "Overflow Error, 0 <= n <= 20 allowed"
	newLine: .asciiz "\n"

.text
	main:
		li $v0, 4
		la $a0, myMessage
		syscall
		
		li $v0, 5
		syscall
		
		move $s0, $v0
		
		li $t0, 0
		li $t1, 1
		li $t2, 2
		li $t3, 4
		
		sw $t0, fibArray($t0)
		sw $t1, fibArray($t3)
		
		li $t3, 20
		
		beq $s0, $t0, exit
		blt $s0, $t0, underflow
		bgt $s0, $t3, overflow
		beq $s0, $t1, print_array
		beq $s0, $t2, print_array
		
		li $t3, 8
		mul $t4, $s0, 4
		
		fib_loop:
			beq $t3, $t4, print_array
			
			add $t2, $t1, $t0
			move $t0, $t1
			move $t1, $t2
			
			sw $t2, fibArray($t3)
			
			addi $t3, $t3, 4
			
			j fib_loop
		
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

