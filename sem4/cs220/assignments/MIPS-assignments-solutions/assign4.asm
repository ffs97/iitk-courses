.data
	message1n: .asciiz "Input the first Rational Number\n\tNumerator: "
	message1d: .asciiz "\tDemominator: "
	message2n: .asciiz "Input the second Rational Number\n\tNumerator: "
	message2d: .asciiz "\tDenominator: "
	
	answer: .asciiz "+0.00e+00"
	
.text
	main:
		#Take input for first rational number and calculating its value
			li $v0, 4
			la $a0, message1n
			syscall
			
			li $v0, 7
			syscall
			mov.d $f4, $f0 #Store numerator in $f4
			
			li $v0, 4
			la $a0, message1d
			syscall
			
			li $v0, 7
			syscall
			mov.d $f6, $f0 #Store denominator in $f6

			div.d $f4, $f4, $f6
		
		#Take input for second rational number
			li $v0, 4
			la $a0, message2n
			syscall
			
			li $v0, 7
			syscall
			mov.d $f6, $f0 #Store numerator in $f6
			
			li $v0, 4
			la $a0, message2d
			syscall
			
			li $v0, 7
			syscall
			mov.d $f8, $f0 #Store denominator in $f8

			div.d $f6, $f6, $f8
		
		#Calculating value of result
			add.d $f4, $f4, $f6

		la $s0, answer #Save address of answer string in $s0
		
		# Check if number is negative or positive
			li.d $f2, 0.0
			c.lt.d $f4, $f2
			bc1f continue
				li $t1, '-'
				sb $t1, 0($s0)
				abs.d $f4, $f4
		
		continue:
		
		li.d $f6, 10.0 #$f6 is for comparing purposes
		li.d $f8, 1.0 #$f8 is for comparing purposes
		li $t0, 0 #$t0 is for the exponent value to be achieved
		loop:
			c.lt.d $f4, $f8 #Comparing Value if less than 1
			bc1t subtractExponent #Jump to branch if contition true
			
			c.le.d $f4, $f6 #Camparing Value if less than 10
			bc1f addExponent  #Jump to branch if contition false
			
			j exit
			
			addExponent: #If value is greater than 10, we need to divide by 10 and add one to exponent
				add $t0, $t0, 1
				div.d $f4, $f4, $f6
				
				j loop
				
			subtractExponent: #If value is less than 1, we need to multiply by 10 and subtract one from exponent
				add $t0, $t0, -1
				mul.d $f4, $f4, $f6
				
				j loop
				
		exit:
			#Round off of 4 decimal places and store in $t1
				li.d $f6, 100.0
				mul.d $f4, $f4, $f6
				
				round.w.d $f6, $f4
				mfc1.d $t1, $f6
			
			li $t3, 10
			
			#Store the decimal numbers into the string
				div $t1, $t3
				mfhi $t1 #Get the number at the second decimal place
				add $t1, $t1, 48 #Convert number to its ascii
				sb $t1, 4($s0) #Store into the string
				mflo $t1
				
				div $t1, $t3
				mfhi $t1 #Get the number at the first decimal place
				add $t1, $t1, 48 #Convert to ascii
				sb $t1, 3($s0) #Store into the string
				mflo $t1 #Get the number before the decimal place
				add $t1, $t1, 48 #Convert to ascii
				sb $t1, 1($s0) #Store into the string
			
			bgez $t0, positiveExponent
			#Skip if exponent is positive	
				abs $t0, $t0 #Change exponent to absolute value
				li $t1, '-'
				sb $t1, 6($s0) #Change sign of exponent in string to -ve
			
			positiveExponent:
			#Storing the exponent to the string
				div $t0, $t3
				mfhi $t1 #Get the second digit of exponent
				add $t1, $t1, 48 #Convert to ascii
				sb $t1, 8($s0) #Store into the string
				mflo $t1 #Get the first digit of exponent (can be 0)
				add $t1, $t1, 48 #Convert to ascii
				sb $t1, 7($s0) #Store into the string
			
			#Print the string
				li $v0, 4
				la $a0, answer
				syscall

			li $v0, 10
			syscall
