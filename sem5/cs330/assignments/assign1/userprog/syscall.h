/* syscalls.h 
 * 	Nachos system call interface.  These are Nachos kernel operations
 * 	that can be invoked from user programs, by trapping to the kernel
 *	via the "syscall" instruction.
 *
 *	This file is included by user programs and by the Nachos kernel. 
 *
 * Copyright (c) 1992-1993 The Regents of the University of California.
 * All rights reserved.  See copyright.h for copyright notice and limitation 
 * of liability and disclaimer of warranty provisions.
 */

#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "copyright.h"

/* system call codes -- used by the stubs to tell the kernel which system call
 * is being asked for
 */
#define SysCall_Halt		0
#define SysCall_Exit		1
#define SysCall_Exec		2
#define SysCall_Join		3
#define SysCall_Create		4
#define SysCall_Open		5
#define SysCall_Read		6
#define SysCall_Write		7
#define SysCall_Close		8
#define SysCall_Fork		9
#define SysCall_Yield		10

// New syscalls defined by Mainak

#define SysCall_PrintInt	11
#define SysCall_PrintChar	12
#define SysCall_PrintString	13

#define SysCall_GetReg		14
#define SysCall_GetPA		15
#define SysCall_GetPID		16
#define SysCall_GetPPID		17

#define SysCall_Sleep		18

#define SysCall_Time		19

#define SysCall_PrintIntHex  	20

#define SysCall_NumInstr	50

#ifndef IN_ASM

/* The system call interface.  These are the operations the Nachos
 * kernel needs to support, to be able to run user programs.
 *
 * Each of these is invoked by a user program by simply calling the 
 * procedure; an assembly language stub stuffs the system call code
 * into a register, and traps to the kernel.  The kernel procedures
 * are then invoked in the Nachos kernel, after appropriate error checking, 
 * from the system call entry point in exception.cc.
 */

/* Stop Nachos, and print out performance stats */
void syscall_wrapper_Halt();		
 

/* Address space control operations: Exit, Exec, and Join */

/* This user program is done (status = 0 means exited normally). */
void syscall_wrapper_Exit(int status);	

/* A unique identifier for an executing user program (address space) */
/* This is same as PID. */
typedef int SpaceId;	
 
/* Run the executable, stored in the Nachos file "name"
 */
void syscall_wrapper_Exec(char *name);
 
/* Only return once the the user program "id" has finished.  
 * Return the exit status.
 */
int syscall_wrapper_Join(SpaceId id); 	
 

/* File system operations: Create, Open, Read, Write, Close
 * These functions are patterned after UNIX -- files represent
 * both files *and* hardware I/O devices.
 *
 * If this assignment is done before doing the file system assignment,
 * note that the Nachos file system has a stub implementation, which
 * will work for the purposes of testing out these routines.
 */
 
/* A unique identifier for an open Nachos file. */
typedef int OpenFileId;	

/* when an address space starts up, it has two open files, representing 
 * keyboard input and display output (in UNIX terms, stdin and stdout).
 * Read and Write can be used directly on these, without first opening
 * the console device.
 */

#define ConsoleInput	0  
#define ConsoleOutput	1  
 
/* Create a Nachos file, with "name" */
void syscall_wrapper_Create(char *name);

/* Open the Nachos file "name", and return an "OpenFileId" that can 
 * be used to read and write to the file.
 */
OpenFileId syscall_wrapper_Open(char *name);

/* Write "size" bytes from "buffer" to the open file. */
void syscall_wrapper_Write(char *buffer, int size, OpenFileId id);

/* Read "size" bytes from the open file into "buffer".  
 * Return the number of bytes actually read -- if the open file isn't
 * long enough, or if it is an I/O device, and there aren't enough 
 * characters to read, return whatever is available (for I/O devices, 
 * you should always wait until you can return at least one character).
 */
int syscall_wrapper_Read(char *buffer, int size, OpenFileId id);

/* Close the file, we're done reading and writing to it. */
void syscall_wrapper_Close(OpenFileId id);



/* User-level thread operations: Fork and Yield.  To allow multiple
 * threads to run within a user program. 
 */

/* Fork a thread. Returns child pid to parent and zero to child.
 */
int syscall_wrapper_Fork(void);

/* Yield the CPU to another runnable thread, whether in this address space 
 * or not. 
 */
void syscall_wrapper_Yield();		

// New definitions

void syscall_wrapper_PrintInt (int x);

void syscall_wrapper_PrintChar (char x);

void syscall_wrapper_PrintString (char *x);

void syscall_wrapper_PrintIntHex (int x);

int syscall_wrapper_GetReg (int regno);

int syscall_wrapper_GetPA (unsigned vaddr);

int syscall_wrapper_GetPID (void);

int syscall_wrapper_GetPPID (void);

void syscall_wrapper_Sleep (unsigned);

int syscall_wrapper_GetTime (void);

int syscall_wrapper_GetNumInstr (void);

#endif /* IN_ASM */

#endif /* SYSCALL_H */
