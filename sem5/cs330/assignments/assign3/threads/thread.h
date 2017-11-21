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

#define MAX_CHILD_COUNT 100

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
#define StackSize (4 * 1024) // in words

// NachOSThread state
enum ThreadStatus
{
  JUST_CREATED,
  RUNNING,
  READY,
  BLOCKED
};

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

class NachOSThread
{
private:
  // NOTE: DO NOT CHANGE the order of these first two members.
  // THEY MUST be in this position for SWITCH to work.
  int *stackTop;                      // the current stack pointer
  int machineState[MachineStateSize]; // all registers except for stackTop

public:
  NachOSThread(char *debugName, int nice); // initialize a NachOSThread
  ~NachOSThread();                         // deallocate a NachOSThread
  // NOTE -- thread being deleted
  // must not be running when delete
  // is called

  // basic thread operations

  void ThreadFork(VoidFunctionPtr func, int arg); // Make thread run (*func)(arg)
  void YieldCPU();                                // Relinquish the CPU if any
  // other thread is runnable
  void PutThreadToSleep(); // Put the thread to sleep and
  // relinquish the processor
  void FinishThread(); // The thread is done executing

  void Exit(bool terminateSim, int exitcode); // Invoked when a thread calls
  // Exit. The argument specifies
  // if all threads have called
  // Exit, in which case the
  // simulation should be
  // terminated.

  void CheckOverflow(); // Check if thread has
  // overflowed its stack
  void setStatus(ThreadStatus st) { status = st; }

  ThreadStatus getStatus(void) { return status; }

  char *getName() { return (name); }

  void Print() { printf("%s, ", name); }

  inline int GetPID(void) { return pid; }

  inline int GetPPID(void) { return ppid; }

  void SetChildExitCode(int childpid, int exitcode); // Called by an exiting child thread

  int CheckIfChild(int childpid); // Called by Join to verify that the caller
  // is joining a legitimate child.

  int JoinWithChild(int whichchild); // Called by SysCall_Join

  void RegisterNewChild(int childpid)
  {
    childpidArray[childcount] = childpid;
    childcount++;
    ASSERT(childcount < MAX_CHILD_COUNT);
  }

  void ResetReturnValue(); // Used by SysCall_Fork to set the return value of child to zero
  void
  Schedule(); // Called by SysCall_Fork to enqueue the newly created child thread in the ready queue

  void CreateThreadStack(VoidFunctionPtr func,
                         int arg); // Allocate a stack for the simulated thread context. The thread starts execution at
  // func (in kernel space) with input argument arg.

  void
  Startup(); // Called by the startup function of SysCall_Fork to cleanly start a forked child after it is scheduled

  void SortedInsertInWaitQueue(unsigned when); // Called by SysCall_Sleep handler

  void IncInstructionCount();

  unsigned GetInstructionCount();

  void SetWaitStartTime(int ticks);

  int GetWaitStartTime(void);

  void SetCPUBurstStartTime(int ticks);

  int GetCPUBurstStartTime(void);

  void SetBasePriority(int p);

  int GetBasePriority(void);

  void SetPriority(int p);

  int GetPriority(void);

  void SetUsage(int usage);

  int GetUsage(void);

private:
  // some of the private data for this class is listed above

  int *stack; // Bottom of the stack
  // NULL if this is the main thread
  // (If NULL, don't deallocate stack)
  ThreadStatus status; // ready, running or blocked

  char *name;

  int pid, ppid; // My pid and my parent's pid

  int childpidArray[MAX_CHILD_COUNT]; // My children
  int childexitcode[MAX_CHILD_COUNT]; // Exit code of my children (return values for Join calls)
  bool exitedChild[MAX_CHILD_COUNT];  // Which children have exited?
  unsigned childcount;                // Count of children

  int waitchild_id; // Child I am waiting on (as a result of a Join call)

  int wait_start_time;  // Start tick of wait in ready queue
  int burst_start_time; // Start of the current CPU burst

  int basePriority, schedPriority, usage; // Used by the UNIX scheduler
  // schedPriority is also used to store the next burst estimate

  unsigned instructionCount; // Keeps track of the instruction count executed by this thread

#ifdef USER_PROGRAM
  // A thread running a user program actually has *two* sets of CPU registers --
  // one for its state while executing user code, one for its state
  // while executing kernel code.

  int userRegisters[NumTotalRegs]; // user-level CPU register state
  bool stateRestored;

public:
  void SaveUserState();    // save user-level register state
  void RestoreUserState(); // restore user-level register state

  ProcessAddressSpace *space; // User code this thread is running.
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
