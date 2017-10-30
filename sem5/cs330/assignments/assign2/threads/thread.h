// thread.h
//	Data structures for managing threads.  A thread represents
//	sequential execution of code within a program.
//	So the state of a thread includes the program counter,
//	the processor registers, and the execution stack.
//
// 	Note that because we allocate a fixed size stack for each
//	thread, it is possible to overflow the stack -- for instance,
//	by recursing to too deep a level.  The most common reason
//	for this occuring is allocating large data structures
//	on the stack.  For instance, this will cause problems:
//
//		void foo() { int buf[1000]; ...}
//
//	Instead, you should allocate all data structures dynamically:
//
//		void foo() { int *buf = new int[1000]; ...}
//
//
// 	Bad things happen if you overflow the stack, and in the worst
//	case, the problem may not be caught explicitly.  Instead,
//	the only symptom may be bizarre segmentation faults.  (Of course,
//	other problems can cause seg faults, so that isn't a sure sign
//	that your thread stacks are too small.)
//
//	One thing to try if you find yourself with seg faults is to
//	increase the size of thread stack -- ThreadStackSize.
//
//  	In this interface, forking a thread takes two steps.
//	We must first allocate a data structure for it: "t = new NachOSThread".
//	Only then can we do the fork: "t->fork(f, arg)".
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef THREAD_H
#define THREAD_H

#include "copyright.h"
#include "utility.h"


#ifdef USER_PROGRAM
#include "machine.h"
#include "addrspace.h"
#endif

// CPU register state to be saved on context switch.
// The SPARC and MIPS only need 10 registers, but the Snake needs 18.
// For simplicity, this is just the max over all architectures.
#define MachineStateSize 18


// Size of the thread's private execution stack.
// WATCH OUT IF THIS ISN'T BIG ENOUGH!!!!!
#define StackSize    (4 * 1024)    // in words


// Thread state
enum ThreadStatus {
    JUST_CREATED, RUNNING, READY, BLOCKED
};


/* ----------------------- CUSTOM ----------------------- */
#include "list.h"
/* ----------------------- CUSTOM ----------------------- */

// external function, dummy routine whose sole job is to call NachOSThread::Print
extern void ThreadPrint(int arg);

// The following class defines a "thread control block" -- which
// represents a single thread of execution.
//
//  Every thread has:
//     an execution stack for activation records ("stackTop" and "stack")
//     space to save CPU registers while not running ("machineState")
//     a "status" (running/ready/blocked)
//
//  Some threads also belong to a user address space; threads
//  that only run in the kernel have a NULL address space.

class NachOSThread {
private:
    // NOTE: DO NOT CHANGE the order of these first two members.
    // THEY MUST be in this position for SWITCH to work.
    int *stackTop;             // the current stack pointer
    int machineState[MachineStateSize];  // all registers except for stackTop

public:
    NachOSThread(char *debugName);        // initialize a Thread
    NachOSThread(char *threadName, int baseSchedulingPriority);

    ~NachOSThread();                // deallocate a Thread
    // NOTE -- thread being deleted
    // must not be running when delete
    // is called

    // basic thread operations

    void ThreadFork(VoidFunctionPtr func, int arg);    // Make thread run (*func)(arg)
    void YieldCPU();                // Relinquish the CPU if any
    // other thread is runnable
    void PutThreadToSleep();                // Put the thread to sleep and
    // relinquish the processor
    void FinishThread();                // The thread is done executing

    void CheckOverflow();            // Check if thread has
    // overflowed its stack
    void setStatus(ThreadStatus st);


    char *getName() { return (name); }


    void Print() { printf("%s, ", name); }


    /* ----------------------- CUSTOM ----------------------- */
    int GetPID() { return pid; }


    int GetPPID() { return ppid; }


    void UpdateParent(NachOSThread *parentThread);

    void AddChild(NachOSThread *childThread);

    void AddChildExitCode(int exCode, int cpid);

    void AddExitCode(int exCode);

    int GetChildExitCode(int cpid);

    void WakeUpThread(int childPID);


    void UpdateWaitingThreadPID(int cpid) { waitingThreadPID = cpid; }


#ifdef USER_PROGRAM
    void UpdateRegisterValue(int value, int reg) { userRegisters[reg] = value; }
#endif

    int numInstr;
    /* ----------------------- CUSTOM ----------------------- */

    /* ======================= CUSTOM ======================= */
    void CreateThreadStack(VoidFunctionPtr func, int arg);

    int GetEstimatedBurstTime() { return estimatedBurstTicks; }
    int GetLastBurstStartTicks() { return lastBurstStartTicks; }

    int GetCpuCount() { return cpuCount; }
    int GetBasePriority() { return basePriority; }

    void SetCpuCount(int count) { cpuCount = count; }
    /* ======================= CUSTOM ======================= */

private:
    // some of the private data for this class is listed above

    int *stack;            // Bottom of the stack
    // NULL if this is the main thread
    // (If NULL, don't deallocate stack)
    ThreadStatus status;        // ready, running or blocked
    char *name;
    // Allocate a stack for thread.
    // Used internally by ThreadFork()

    int pid, ppid;            // My pid and my parent's pid

    /* ----------------------- CUSTOM ----------------------- */
    NachOSThread *parent;    // My parent thread

    List *childQueue;
    List *childExitCodeQueue;

    int waitingThreadPID;
    /* ----------------------- CUSTOM ----------------------- */

    /* ======================= CUSTOM ======================= */
    int lastBurstStartTicks;
    int lastWaitStartTicks;

    int numberOfBursts;

    int estimatedBurstTicks;

    int cpuCount;
    int basePriority;
    /* ======================= CUSTOM ======================= */

#ifdef USER_PROGRAM
    // A thread running a user program actually has *two* sets of CPU registers --
// one for its state while executing user code, one for its state
// while executing kernel code.

    int userRegisters[NumTotalRegs];    // user-level CPU register state
    bool stateRestored;

public:
    void SaveUserState();        // save user-level register state
    void RestoreUserState();        // restore user-level register state

    ProcessAddressSpace *space;            // User code this thread is running.
#endif
};

// Magical machine-dependent routines, defined in switch.s

extern "C" {
// First frame on thread execution stack;
//   	enable interrupts
//	call "func"
//	(when func returns, if ever) call ThreadFinish()
void _ThreadRoot();

// Stop running oldThread and start running newThread
void _SWITCH(NachOSThread *oldThread, NachOSThread *newThread);
}

#endif // THREAD_H
