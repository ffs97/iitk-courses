/* Start.s 
 *	Assembly language assist for user programs running on top of Nachos.
 *
 *	Since we don't want to pull in the entire C library, we define
 *	what we need for a user program here, namely Start and the system
 *	calls.
 */

#define IN_ASM
#include "syscall.h"

        .text   
        .align  2

/* -------------------------------------------------------------
 * __start
 *	Initialize running a C program, by calling "main". 
 *
 * 	NOTE: This has to be first, so that it gets loaded at location 0.
 *	The Nachos kernel always starts a program by jumping to location 0.
 * -------------------------------------------------------------
 */

	.globl __start
	.ent	__start
__start:
	jal	main
	move	$4,$0		
	jal	syscall_wrapper_Exit	 /* if we return from main, exit(0) */
	.end __start

/* -------------------------------------------------------------
 * System call stubs:
 *	Assembly language assist to make system calls to the Nachos kernel.
 *	There is one stub per system call, that places the code for the
 *	system call into register r2, and leaves the arguments to the
 *	system call alone (in other words, arg1 is in r4, arg2 is 
 *	in r5, arg3 is in r6, arg4 is in r7)
 *
 * 	The return value is in r2. This follows the standard C calling
 * 	convention on the MIPS.
 * -------------------------------------------------------------
 */

	.globl syscall_wrapper_Halt
	.ent	syscall_wrapper_Halt
syscall_wrapper_Halt:
	addiu $2,$0,SysCall_Halt
	syscall
	j	$31
	.end syscall_wrapper_Halt

	.globl syscall_wrapper_Exit
	.ent	syscall_wrapper_Exit
syscall_wrapper_Exit:
	addiu $2,$0,SysCall_Exit
	syscall
	j	$31
	.end syscall_wrapper_Exit

	.globl syscall_wrapper_Exec
	.ent	syscall_wrapper_Exec
syscall_wrapper_Exec:
	addiu $2,$0,SysCall_Exec
	syscall
	j	$31
	.end syscall_wrapper_Exec

	.globl syscall_wrapper_Join
	.ent	syscall_wrapper_Join
syscall_wrapper_Join:
	addiu $2,$0,SysCall_Join
	syscall
	j	$31
	.end syscall_wrapper_Join

	.globl syscall_wrapper_Create
	.ent	syscall_wrapper_Create
syscall_wrapper_Create:
	addiu $2,$0,SysCall_Create
	syscall
	j	$31
	.end syscall_wrapper_Create

	.globl syscall_wrapper_Open
	.ent	syscall_wrapper_Open
syscall_wrapper_Open:
	addiu $2,$0,SysCall_Open
	syscall
	j	$31
	.end syscall_wrapper_Open

	.globl syscall_wrapper_Read
	.ent	syscall_wrapper_Read
syscall_wrapper_Read:
	addiu $2,$0,SysCall_Read
	syscall
	j	$31
	.end syscall_wrapper_Read

	.globl syscall_wrapper_Write
	.ent	syscall_wrapper_Write
syscall_wrapper_Write:
	addiu $2,$0,SysCall_Write
	syscall
	j	$31
	.end syscall_wrapper_Write

	.globl syscall_wrapper_Close
	.ent	syscall_wrapper_Close
syscall_wrapper_Close:
	addiu $2,$0,SysCall_Close
	syscall
	j	$31
	.end syscall_wrapper_Close

	.globl syscall_wrapper_Fork
	.ent	syscall_wrapper_Fork
syscall_wrapper_Fork:
	addiu $2,$0,SysCall_Fork
	syscall
	j	$31
	.end syscall_wrapper_Fork

	.globl syscall_wrapper_Yield
	.ent	syscall_wrapper_Yield
syscall_wrapper_Yield:
	addiu $2,$0,SysCall_Yield
	syscall
	j	$31
	.end syscall_wrapper_Yield

	.globl syscall_wrapper_PrintInt
	.ent    syscall_wrapper_PrintInt
syscall_wrapper_PrintInt:
        addiu $2,$0,SysCall_PrintInt
        syscall
        j       $31
	.end syscall_wrapper_PrintInt

	.globl syscall_wrapper_PrintChar
	.ent    syscall_wrapper_PrintChar
syscall_wrapper_PrintChar:
        addiu $2,$0,SysCall_PrintChar
        syscall
        j       $31
	.end syscall_wrapper_PrintChar

	.globl syscall_wrapper_PrintString
	.ent    syscall_wrapper_PrintString
syscall_wrapper_PrintString:
        addiu $2,$0,SysCall_PrintString
        syscall
        j       $31
	.end syscall_wrapper_PrintString

	.globl syscall_wrapper_GetReg
	.ent    system_callGetReg
syscall_wrapper_GetReg:
	addiu $2,$0,SysCall_GetReg
	syscall
	j       $31
	.end syscall_wrapper_GetReg

	.globl syscall_wrapper_GetPA
	.ent    syscall_wrapper_GetPA
syscall_wrapper_GetPA:
	addiu $2,$0,SysCall_GetPA
	syscall
	j       $31
	.end syscall_wrapper_GetPA

	.globl syscall_wrapper_GetPID
	.ent    syscall_wrapper_GetPID
syscall_wrapper_GetPID:
	addiu $2,$0,SysCall_GetPID
	syscall
	j       $31
	.end syscall_wrapper_GetPID

	.globl syscall_wrapper_GetPPID
	.ent    syscall_wrapper_GetPPID
syscall_wrapper_GetPPID:
	addiu $2,$0,SysCall_GetPPID
	syscall
	j       $31
	.end syscall_wrapper_GetPPID

	.globl syscall_wrapper_Sleep
	.ent    syscall_wrapper_Sleep
syscall_wrapper_Sleep:
	addiu $2,$0,SysCall_Sleep
	syscall
	j       $31
	.end syscall_wrapper_Sleep

	.globl syscall_wrapper_GetTime
	.ent    syscall_wrapper_GetTime
syscall_wrapper_GetTime:
	addiu $2,$0,SysCall_Time
	syscall
	j       $31
	.end syscall_wrapper_GetTime

	.globl syscall_wrapper_GetNumInstr
	.ent    syscall_wrapper_GetNumInstr
syscall_wrapper_GetNumInstr:
	addiu $2,$0,SysCall_NumInstr
	syscall
	j	$31
	.end syscall_wrapper_GetNumInstr

	.globl syscall_wrapper_PrintIntHex
	.ent    syscall_wrapper_PrintIntHex
syscall_wrapper_PrintIntHex:
	addiu $2,$0,SysCall_PrintIntHex
	syscall
	j	$31
	.end syscall_wrapper_PrintIntHex

/* dummy function to keep gcc happy */
        .globl  __main
        .ent    __main
__main:
        j       $31
        .end    __main

