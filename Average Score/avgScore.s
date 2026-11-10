.data 

orig: .space 100	# In terms of bytes (25 elements * 4 bytes each)
sorted: .space 100

str0: .asciiz "Enter the number of assignments (between 1 and 25): "
str1: .asciiz "Enter score: "
str2: .asciiz "Original scores: "
str3: .asciiz "Sorted scores (in descending order): "
str4: .asciiz "Enter the number of (lowest) scores to drop: "
str5: .asciiz "Average (rounded down) with dropped scores removed: "
str6:  .asciiz "All scores dropped!"

.text 

# This is the main program.
# It first asks user to enter the number of assignments.
# It then asks user to input the scores, one at a time.
# It then calls selSort to perform selection sort.
# It then calls printArray twice to print out contents of the original and sorted scores.
# It then asks user to enter the number of (lowest) scores to drop.
# It then calls calcSum on the sorted array with the adjusted length (to account for dropped scores).
# It then prints out average score with the specified number of (lowest) scores dropped from the calculation.
main: 
	addi $sp, $sp -4	
	sw $ra, 0($sp)
	#la $a0, str0 
	#li $v0, 4 
	#syscall 
	#li $v0, 5	# Read the number of scores from user
	#syscall
	
	# Your code here to handle invalid number of scores (can't be less than 1 or greater than 25)

# -- Extra Credit: -- 
input_numScores:
	la $a0, str0 
	li $v0, 4 
	syscall 
	li $v0, 5	# Read the number of scores from user
	syscall
	
	# handle invalid number of scores (can't be less than 1 or greater than 25)
	slti $t0, $v0, 1			#$t0 = 1 if $v0 < 1
	bne $t0, $zero, input_numScores		#If < 1, ask again
	
	slti $t0, $v0, 26			#$t0 = 1 if $v0 < 26 (which means $v0 <= 25)
	beq $t0, $zero, input_numScores		#If >= 26 (i.e., > 25), ask again

	
	move $s0, $v0	# $s0 = numScores
	move $t0, $0
	la $s1, orig	# $s1 = orig
	la $s2, sorted	# $s2 = sorted
loop_in:
	li $v0, 4 
	la $a0, str1 
	syscall 
	sll $t1, $t0, 2
	add $t1, $t1, $s1
	li $v0, 5	# Read elements from user
	syscall
	sw $v0, 0($t1)
	addi $t0, $t0, 1	
	bne $t0, $s0, loop_in
	
	move $a0, $s0
	jal selSort	# Call selSort to perform selection sort in original array
	
	li $v0, 4
	la $a0, str2 
	syscall	
	move $a0, $s1	# More efficient than la $a0, orig
	move $a1, $s0
	jal printArray	# Print original scores
	li $v0, 4 
	la $a0, str3
	syscall 
	move $a0, $s2	# More efficient than la $a0, sorted
	jal printArray	# Print sorted scores
	

input_drop:
	li $v0, 4 
	la $a0, str4 
	syscall 
	li $v0, 5	# Read the number of (lowest) scores to drop
	syscall	
	
	# Your code here to handle invalid number of (lowest) scores to drop (can't be less than 0, or 
	# greater than the number of scores). Also, handle the case when number of (lowest) scores to drop 
	# equals the number of scores. 

#-- Extra Credit-- 
	slti $t0, $v0, 0 		#$t0 = 1 if $v0 < 0
	bne $t0, $zero, input_drop	#If drop > numScores, ask again
	
	slt $t0, $s0, $v0		#$t0 = 1 if numScores < drop
	bne $t0, $zero, input_drop	#IF drop > numScores, ask again
	
# --Extra Credit --
	#Handle the case when all scores are dropped
	bne $v0, $s0, calc_avg	
	li $v0, 4		
	la $a0, str6		
	syscall			
	j end			
	
calc_avg:
	move $a3, $v0		#a3 = num to drop
	move $a1, $v0		
	sub $a1, $s0, $a1	# a1 = numScores - drop
	move $a0, $s2
	jal calcSum	# Call calcSum to RECURSIVELY compute the sum of scores that are not dropped
	
	# Your code here to compute average and print it (you may also end up having some code here to help 
	# handle the case when number of (lowest) scores to drop equals the number of scores
	sub $t0, $s0, $a3	#t0 = numScores - drop (number of scores used)
	div $v0, $t0		#sum / count
	mflo $t1		#Get quotient (round down) (mflow holds the whole number, and mfhi holds the remainder)
	
	li $v0, 4	
	la $a0, str5	
	syscall		
	li $v0, 1	
	move $a0, $t1	
	syscall		
	li $v0, 11		#Print new line
	li $a0, 10	
	syscall		
	
	
end:	
	lw $ra, 0($sp)	
	addi $sp, $sp 4	
	li $v0, 10 	
	syscall		
	
	
# printList takes in an array and its size as arguments. 
# It prints all the elements in one line with a newline at the end.
printArray:
	# Your implementation of printList here	
	addi $sp, $sp, -12
	sw $ra, 0($sp)	
	sw $s0, 4($sp)	
	sw $s1, 8($sp)	
	
	move $s0, $a0	#$s0 = array address
	move $s1, $a1	#$s1 = size
	li $t0, 0	#$t0 = counter
	
print_loop:
	beq $t0, $s1, print_done
	
	sll $t1, $t0, 2		#$t1 = i * 4
	add $t1, $t1, $s0 	#$t1 = address of array[i]
	lw $a0, 0($t1)		#Load array[i]
	li $v0, 1		#Print integer
	syscall
	
	li $v0, 11		#print space
	li $a0, 32	
	syscall		
	
	addi $t0, $t0, 1
	j print_loop	
	
print_done:
	li $v0, 11		#Print newline
	li $a0, 10	
	syscall		

	lw $ra, 0($sp)	
	lw $s0, 4($sp)	
	lw $s1, 8($sp)	
	addi $sp, $sp, 12

	jr $ra		
	
	
# selSort takes in the number of scores as argument. 
# It performs SELECTION sort in descending order and populates the sorted array
selSort:
	# Your implementation of selSort here
	addi $sp, $sp, -4
	sw $s0, 0($sp)	
	
	move $s0, $a0 		#$s0 = len
	la $t0, orig		#$t0 = orig address
	la $t1, sorted		#$t1 = sorted address
	li $t2, 0		#$t2 = i (counter)
	
	#copyorig to sorted
copy_loop:
	beq $t2, $s0, copy_done
	sll $t3, $t2, 2 	#$t3 = i * 4
	add $t4, $t0, $t3	#$t4 = &orig[i]
	add $t5, $t1, $t3	#$t5 = $sorted[i]
	lw $t6, 0($t4) 		#Load orig[i]
	sw $t6, 0($t5)		#Store to sorted[i]
	addi $t2, $t2, 1
	j copy_loop	

copy_done:
	#Selectoin sort in descending order
	li $t2, 0		#$t2 = i (outer loop)
	addi $t9, $s0, -1	#$t9 = len - 1
	
outer_loop:
	beq $t2, $t9, sort_done	
	
	#Find max element in sorted[i..len-1]
	move $t3, $t2 		#$t3 = maxIndex = i
	addi $t4, $t2, 1	#$t4 = j = i + 1

inner_loop:
	beq $t4, $s0, swap_max	
		
	#load sorted[j] and sorted[maxIndex]
	sll $t5, $t4, 2	
	add $t5, $t5, $t1
	lw $t6, 0($t5)		#$t6 = sorted[j]
	
	sll $t5, $t3, 2	
	add $t5, $t5, $t1
	lw $t7, 0($t5)		#$t7 = sorted[maxIndex]
	
	#If sorted[j] > sorted[maxIndex], update maxIndex
	slt $t8, $t7, $t6	#t8 = 1 if sorted[maxIndex] < sorted[j]
	beq $t8, $zero, skip_update	#If not greater then skip
	move $t3, $t4		#maxIndex = j
	
skip_update:
	addi $t4, $t4, 1
	j inner_loop		
	
swap_max:
	#Swap sorted[maxIndex] and sorted[i]
	sll $t5, $t3, 2		
	add $t5, $t5, $t1
	lw $t6, 0($t5)		#$t6 = sorted[maxIndex]
	
	sll $t7, $t2, 2	
	add $t7, $t7, $t1
	lw $t8, 0($t7)		#$t8 = sorted[i]
	
	sw $t8, 0($t5)		#sorted[maxIndex] = sorted[i]
	sw $t6, 0($t7)		#sorted[i] = temp
	
	addi $t2, $t2, 1
	j outer_loop	

sort_done:
	lw $s0, 0($sp)	
	addi $sp, $sp, 4
	
	jr $ra		
	
	
# calcSum takes in an array and its size as arguments.
# It RECURSIVELY computes and returns the sum of elements in the array.
# Note: you MUST NOT use iterative approach in this function.
calcSum:
	# Your implementation of calcSum here
	addi $sp, $sp, -12	
	sw $ra, 0($sp)		
	sw $a0, 4($sp)		
	sw $a1, 8($sp)		
	
	#Base case: if len <= 0, return 0
	slt $t0, $zero, $a1		#$t0 = 1 if 0 < len
	bne $t0, $zero, recursive_case	#If len > 0, do recursive case

	li $v0, 0		
	j calcSum_done		
	
recursive_case:
	#Recursive case: return calcSum(arr, len-1) + arr[len-1]
	addi $a1, $a1, -1 	#len = len - 1
	jal calcSum		#Recirsive call: calcSum(arr, len-1)
	
	#Now add arr[len-1] to result
	lw $a0, 4($sp)		#Restore array address
	lw $a1, 8($sp)		#Restore original len
	addi $a1, $a1, -1	#len - 1
	sll $t0, $a1, 2		#(len-1) * 4
	add $t0,$t0, $a0	#Address of arr[len-1]
	lw $t1, 0($t0)		#Load arr[len-1]
	add $v0, $v0, $t1 	#sum = calcSum(arr, len-1) + arr[len-1)
	
calcSum_done:
	lw $ra, 0($sp)	
	lw $a0, 4($sp)	
	lw $a1, 8($sp)	
	addi $sp, $sp, 12
	
	jr $ra		
	
