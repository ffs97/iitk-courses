// thread.cc 
//	Routines to manage threads.  There are four main operations:
//
//	ThreadFork -- create a thread to run a procedure concurrently
//		with the caller (this is done in two steps -- first
//		allocate the Thread object, then call ThredFork on it)
//	Finish -- called when the forked procedure finishes, to clean up
//	YieldCPU -- relinquish control over the CPU to another ready thread
//	PutThreadToSleep -- relinquish control over the CPU, but thread is now blocked.
//		In other words, it will not run again, until explicitly 
//		put back on the ready queue.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "thread.h"
#include "switch.h"
#include "synch.h"
#include "system.h"


#define STACK_FENCEPOST 0xdeadbeef    // this is put at the top of the
// execution stack, for detecting
// stack overflows

//----------------------------------------------------------------------
// NachOSThread::NachOSThread
// 	Initialize a thread control block, so that we can then call
//	NachOSThread::ThreadFork.
//
//	"threadName" is an arbitrary string, useful for debugging.
//----------------------------------------------------------------------

NachOSThread::NachOSThread(char *threadName) {
    name = threadName;

    /* ----------------------- CUSTOM ----------------------- */
    pid = ++numThreadsCreated;
    numThreadsCurrent++;

    parent = currentThread;
    if (parent != NULL) {
        ppid = parent->GetPID();
        parent->AddChild(this);
        /* ======================= CUSTOM ======================= */
        basePriority = parent->basePriority;
        cpuCount = parent->cpuCount;
        /* ======================= CUSTOM ======================= */
    }
    else {
        /* ======================= CUSTOM ======================= */
        basePriority = minimumBasePriority;
        cpuCount = 0;
        /* ======================= CUSTOM ======================= */
        ppid = -1;
    }

    numInstr = 0;

    childQueue = new List;
    childExitCodeQueue = new List;

    waitingThreadPID = -1;
    /* ----------------------- CUSTOM ----------------------- */

    /* ======================= CUSTOM ======================= */
    estimatedBurstTicks = 0;

    lastBurstStartTicks = 0;
    lastWaitStartTicks = 0;
    /* ======================= CUSTOM ======================= */

    stackTop = NULL;
    stack = NULL;
    status = JUST_CREATED;
#ifdef USER_PROGRAM
    space = NULL;
    stateRestored = true;
#endif
}


NachOSThread::NachOSThread(char *threadName, int baseSchedulingPriority) {
    name = threadName;

    /* ----------------------- CUSTOM ----------------------- */
    pid = ++numThreadsCreated;
    numThreadsCurrent++;

    /* ======================= CUSTOM ======================= */
    basePriority = baseSchedulingPriority;
    cpuCount = 0;
    /* ======================= CUSTOM ======================= */

    parent = currentThread;
    if (parent != NULL) {
        ppid = parent->GetPID();
        parent->AddChild(this);
    }
    else {
        ppid = -1;
    }

    numInstr = 0;

    childQueue = new List;
    childExitCodeQueue = new List;

    waitingThreadPID = -1;
    /* ----------------------- CUSTOM ----------------------- */

    /* ======================= CUSTOM ======================= */
    estimatedBurstTicks = 0;

    lastBurstStartTicks = 0;
    lastWaitStartTicks = 0;
    /* ======================= CUSTOM ======================= */

    stackTop = NULL;
    stack = NULL;
    status = JUST_CREATED;
#ifdef USER_PROGRAM
    space = NULL;
    stateRestored = true;
#endif
}

//----------------------------------------------------------------------
// NachOSThread::~NachOSThread
// 	De-allocate a thread.
//
// 	NOTE: the current thread *cannot* delete itself directly,
//	since it is still running on the stack that we need to delete.
//
//      NOTE: if this is the main thread, we can't delete the stack
//      because we didn't allocate it -- we got it automatically
//      as part of starting up Nachos.
//----------------------------------------------------------------------

NachOSThread::~NachOSThread() {
    DEBUG('t', "Deleting thread \"%s\"\n", name);

    ASSERT(this != currentThread);
    if (stack != NULL)
        DeallocBoundedArray((char *) stack, StackSize * sizeof(int));
}

//----------------------------------------------------------------------
// NachOSThread::ThreadFork
// 	Invoke (*func)(arg), allowing caller and callee to execute 
//	concurrently.
//
//	NOTE: although our definition allows only a single integer argument
//	to be passed to the procedure, it is possible to pass multiple
//	arguments by making them fields of a structure, and passing a pointer
//	to the structure as "arg".
//
// 	Implemented as the following steps:
//		1. Allocate a stack
//		2. Initialize the stack so that a call to SWITCH will
//		cause it to run the procedure
//		3. Put the thread on the ready queue
// 	
//	"func" is the procedure to run concurrently.
//	"arg" is a single argument to be passed to the procedure.
//----------------------------------------------------------------------

void
NachOSThread::ThreadFork(VoidFunctionPtr func, int arg) {
    DEBUG('t', "Forking thread \"%s\" with func = 0x%x, arg = %d\n",
          name, (int) func, arg);

    CreateThreadStack(func, arg);

    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    scheduler->MoveThreadToReadyQueue(this);    // MoveThreadToReadyQueue assumes that interrupts

    // are disabled!
    (void) interrupt->SetLevel(oldLevel);
}

//----------------------------------------------------------------------
// NachOSThread::CheckOverflow
// 	Check a thread's stack to see if it has overrun the space
//	that has been allocated for it.  If we had a smarter compiler,
//	we wouldn't need to worry about this, but we don't.
//
// 	NOTE: Nachos will not catch all stack overflow conditions.
//	In other words, your program may still crash because of an overflow.
//
// 	If you get bizarre results (such as seg faults where there is no code)
// 	then you *may* need to increase the stack size.  You can avoid stack
// 	overflows by not putting large data structures on the stack.
// 	Don't do this: void foo() { int bigArray[10000]; ... }
//----------------------------------------------------------------------

void
NachOSThread::CheckOverflow() {
    if (stack != NULL)
#ifdef HOST_SNAKE            // Stacks grow upward on the Snakes
        ASSERT(stack[StackSize - 1] == STACK_FENCEPOST);
#else
    ASSERT(*stack == STACK_FENCEPOST);
#endif
}

//----------------------------------------------------------------------
// NachOSThread::FinishThread
// 	Called by ThreadRoot when a thread is done executing the 
//	forked procedure.
//
// 	NOTE: we don't immediately de-allocate the thread data structure 
//	or the execution stack, because we're still running in the thread 
//	and we're still on the stack!  Instead, we set "threadToBeDestroyed", 
//	so that ProcessScheduler::ScheduleThread() will call the destructor, once we're
//	running in the context of a different thread.
//
// 	NOTE: we disable interrupts, so that we don't get a time slice 
//	between setting threadToBeDestroyed, and going to sleep.
//----------------------------------------------------------------------

void
NachOSThread::FinishThread() {
    (void) interrupt->SetLevel(IntOff);
    ASSERT(this == currentThread);

    DEBUG('t', "Finishing thread \"%s\"\n", getName());

    threadToBeDestroyed = currentThread;

    /* ======================= CUSTOM ======================= */
    if (pid > 1) {
        int totalThreadTicks = stats->totalTicks;

        if (totalThreadTicks < stats->minCompletionTime)
            stats->minCompletionTime = totalThreadTicks;

        if (totalThreadTicks > stats->maxCompletionTime)
            stats->maxCompletionTime = totalThreadTicks;

        double tt = stats->numTotalThreads;
        double ttr = totalThreadTicks / (tt + 1);


        stats->averageCompletionTime = stats->averageCompletionTime * (tt / (tt + 1)) + ttr;
        stats->averageSquareCompletionTime =
                stats->averageSquareCompletionTime * (tt / (tt + 1)) + totalThreadTicks * ttr;

        stats->numTotalThreads++;
    }
    /* ======================= CUSTOM ======================= */

    /* ----------------------- CUSTOM ----------------------- */
    numThreadsCurrent--;

    if (numThreadsCurrent == 0) {
        interrupt->Halt();
    }

    NachOSThread *curChildThread;
    int curChildPID;
    while (!childQueue->IsEmpty()) {
        curChildThread = (NachOSThread *) childQueue->SortedRemove(&curChildPID);
        curChildThread->UpdateParent(NULL);
    }


    if (parent != NULL) { parent->WakeUpThread(this->GetPID()); }
    /* ----------------------- CUSTOM ----------------------- */

    threadToBeDestroyed->PutThreadToSleep();
}

//----------------------------------------------------------------------
// NachOSThread::YieldCPU
// 	Relinquish the CPU if any other thread is ready to run.
//	If so, put the thread on the end of the ready list, so that
//	it will eventually be re-scheduled.
//
//	NOTE: returns immediately if no other thread on the ready queue.
//	Otherwise returns when the thread eventually works its way
//	to the front of the ready list and gets re-scheduled.
//
//	NOTE: we disable interrupts, so that looking at the thread
//	on the front of the ready list, and switching to it, can be done
//	atomically.  On return, we re-set the interrupt level to its
//	original state, in case we are called with interrupts disabled. 
//
// 	Similar to NachOSThread::PutThreadToSleep(), but a little different.
//----------------------------------------------------------------------

void
NachOSThread::YieldCPU() {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    ASSERT(this == currentThread);

    DEBUG('t', "Yielding thread \"%s\"\n", getName());

    /* ======================= CUSTOM ======================= */
    scheduler->MoveThreadToReadyQueue(this);
    NachOSThread *nextThread = scheduler->SelectNextReadyThread();
    scheduler->ScheduleThread(nextThread);
    /* ======================= CUSTOM ======================= */


//    nextThread = scheduler->SelectNextReadyThread();
//    if (nextThread != NULL) {
//        scheduler->MoveThreadToReadyQueue(this);
//        scheduler->ScheduleThread(nextThread);
//    }
//    /* ======================= CUSTOM ======================= */
//    else {
//        int t = stats->totalTicks - lastBurstStartTicks;
//
//        if (t != 0) {
//            if (schedulerType == SHORTEST_BURST) {
//                estimatedBurstTicks = (int) ((1 - ALPHA) * estimatedBurstTicks + ALPHA * t);
//            }
//
//            totalBurstTicks += t;
//            stats->totalBurstTicks += t;
//
//            printf("\n>> Burst: %d\n", t);
//
//            if (t > stats->maxBurstTicks)
//                stats->maxBurstTicks = t;
//
//            if (t < stats->minBurstTicks)
//                stats->minBurstTicks = t;
//
//            stats->numTotalBursts++;
//
//            lastBurstStartTicks = stats->totalTicks;
//        }
//    }
    /* ======================= CUSTOM ======================= */
    (void) interrupt->SetLevel(oldLevel);
}


//----------------------------------------------------------------------
// NachOSThread::PutThreadToSleep
// 	Relinquish the CPU, because the current thread is blocked
//	waiting on a synchronization variable (Semaphore, Lock, or Condition).
//	Eventually, some thread will wake this thread up, and put it
//	back on the ready queue, so that it can be re-scheduled.
//
//	NOTE: if there are no threads on the ready queue, that means
//	we have no thread to run.  "Interrupt::Idle" is called
//	to signify that we should idle the CPU until the next I/O interrupt
//	occurs (the only thing that could cause a thread to become
//	ready to run).
//
//	NOTE: we assume interrupts are already disabled, because it
//	is called from the synchronization routines which must
//	disable interrupts for atomicity.   We need interrupts off 
//	so that there can't be a time slice between pulling the first thread
//	off the ready list, and switching to it.
//----------------------------------------------------------------------
void
NachOSThread::PutThreadToSleep() {
    NachOSThread *nextThread;

    ASSERT(this == currentThread);
    ASSERT(interrupt->getLevel() == IntOff);

    DEBUG('t', "Sleeping thread \"%s\"\n", getName());

    setStatus(BLOCKED);
    while ((nextThread = scheduler->SelectNextReadyThread()) == NULL)
        interrupt->Idle();    // no one to run, wait for an interrupt

    scheduler->ScheduleThread(nextThread); // returns when we've been signalled
}

//----------------------------------------------------------------------
// ThreadFinish, InterruptEnable, ThreadPrint
//	Dummy functions because C++ does not allow a pointer to a member
//	function.  So in order to do this, we create a dummy C function
//	(which we can pass a pointer to), that then simply calls the 
//	member function.
//----------------------------------------------------------------------

static void ThreadFinish() { currentThread->FinishThread(); }


static void InterruptEnable() { interrupt->Enable(); }


void ThreadPrint(int arg) {
    NachOSThread *t = (NachOSThread *) arg;
    t->Print();
}

//----------------------------------------------------------------------
// NachOSThread::CreateThreadStack
//	Allocate and initialize an execution stack.  The stack is
//	initialized with an initial stack frame for ThreadRoot, which:
//		enables interrupts
//		calls (*func)(arg)
//		calls NachOSThread::FinishThread
//
//	"func" is the procedure to be forked
//	"arg" is the parameter to be passed to the procedure
//----------------------------------------------------------------------

void
NachOSThread::CreateThreadStack(VoidFunctionPtr func, int arg) {
    stack = (int *) AllocBoundedArray(StackSize * sizeof(int));

#ifdef HOST_SNAKE
    // HP stack works from low addresses to high addresses
    stackTop = stack + 16;    // HP requires 64-byte frame marker
    stack[StackSize - 1] = STACK_FENCEPOST;
#else
    // i386 & MIPS & SPARC stack works from high addresses to low addresses
#ifdef HOST_SPARC
    // SPARC stack must contains at least 1 activation record to start with.
    stackTop = stack + StackSize - 96;
#else  // HOST_MIPS  || HOST_i386
    stackTop = stack + StackSize - 4;    // -4 to be on the safe side!
#ifdef HOST_i386
    // the 80386 passes the return address on the stack.  In order for
    // SWITCH() to go to ThreadRoot when we switch to this thread, the
    // return addres used in SWITCH() must be the starting address of
    // ThreadRoot.
    *(--stackTop) = (int) _ThreadRoot;
#endif
#endif  // HOST_SPARC
    *stack = STACK_FENCEPOST;
#endif  // HOST_SNAKE

    machineState[PCState] = (int) _ThreadRoot;
    machineState[StartupPCState] = (int) InterruptEnable;
    machineState[InitialPCState] = (int) func;
    machineState[InitialArgState] = arg;
    machineState[WhenDonePCState] = (int) ThreadFinish;
}


#ifdef USER_PROGRAM
#include "machine.h"

//----------------------------------------------------------------------
// NachOSThread::SaveUserState
//	Save the CPU state of a user program on a context switch.
//
//	Note that a user program thread has *two* sets of CPU registers -- 
//	one for its state while executing user code, one for its state 
//	while executing kernel code.  This routine saves the former.
//----------------------------------------------------------------------

void
NachOSThread::SaveUserState() {
    if (stateRestored) {
        for (int i = 0; i < NumTotalRegs; i++)
            userRegisters[i] = machine->ReadRegister(i);
        stateRestored = false;
    }
}

//----------------------------------------------------------------------
// NachOSThread::RestoreUserSLtate
//	Restore the CPU state of a user program on a context switch.
//
//	Note that a user program thread has *two* sets of CPU registers --
//	one for its state while executing user code, one for its state
//	while executing kernel code.  This routine restores the former.
//----------------------------------------------------------------------

void
NachOSThread::RestoreUserState() {
    for (int i = 0; i < NumTotalRegs; i++)
        machine->WriteRegister(i, userRegisters[i]);
    stateRestored = true;
}
#endif


/* ----------------------- CUSTOM ----------------------- */
void
NachOSThread::UpdateParent(NachOSThread *parentThread) {
    parent = parentThread;

    if (parent == NULL) {
        ppid = -1;
    }
    else {
        ppid = parent->GetPID();
    }
}


void
NachOSThread::AddChild(NachOSThread *childThread) {
    childQueue->SortedInsert((void *) childThread, childThread->GetPID());
}


void
NachOSThread::AddChildExitCode(int exCode, int cpid) {
    childExitCodeQueue->SortedInsert((void *) exCode, cpid);

}


void
NachOSThread::WakeUpThread(int childPID) {
    if (status == BLOCKED && this->waitingThreadPID == childPID) {
        waitingThreadPID = -1;

#ifdef USER_PROGRAM
        UpdateRegisterValue(GetChildExitCode(childPID), 2);
#endif

        scheduler->MoveThreadToReadyQueue(this);
    }

}


void
NachOSThread::AddExitCode(int exCode) {
    if (parent != NULL) {
        parent->AddChildExitCode(exCode, pid);
    }
}


int
NachOSThread::GetChildExitCode(int cpid) {
    int exitCode = -1;

    List *tempQueue = new List;

    NachOSThread *curChildThread;
    int curChildPID;
    while (!childQueue->IsEmpty()) {
        curChildThread = (NachOSThread *) childQueue->SortedRemove(&curChildPID);
        tempQueue->SortedInsert((void *) curChildThread, curChildPID);
        if (curChildPID == cpid) {
            exitCode = -2;
        }
    }

    delete childQueue;
    childQueue = tempQueue;

    tempQueue = new List;

    int curChildExitCode;
    while (!childExitCodeQueue->IsEmpty()) {
        curChildExitCode = (int) childExitCodeQueue->SortedRemove(&curChildPID);
        tempQueue->SortedInsert((void *) curChildExitCode, curChildPID);
        if (curChildPID == cpid) {
            exitCode = curChildExitCode;
        }
    }

    delete childExitCodeQueue;
    childExitCodeQueue = tempQueue;

    return exitCode;
}
/* ----------------------- CUSTOM ----------------------- */

/* ======================= CUSTOM ======================= */
void
NachOSThread::setStatus(ThreadStatus st) {
    if (status == JUST_CREATED) {
        printf("\nPID %d: CREATED to %s at Tick %d\n", pid, (st == RUNNING) ? "RUNNING" : "READY", stats->totalTicks);
    }
    else if (status == READY) {
        int t = stats->totalTicks - lastWaitStartTicks;

        printf("\nPID %d: READY to %s at Tick %d\n", pid, (st == RUNNING) ? "RUNNING" : "UNKNOWN", stats->totalTicks);

        stats->totalWaitTicks += t;
    }
    else if (status == RUNNING) {
        int t = stats->totalTicks - lastBurstStartTicks;

        printf("\nPID %d: RUNNING to %s at Tick %d; Burst Length: %d\n", pid, (st == READY) ? "READY" : "BLOCKED", stats->totalTicks, t);

        SetCpuCount(t >> 1);
        printf("\nPID %d: Base Priority is %d and CPU Count is %d\n", pid, basePriority, cpuCount);
        scheduler->UpdatePriorities();

        if (t != 0) {
            if (schedulerType == SHORTEST_BURST) {
                estimatedBurstTicks = (int) ((1 - ALPHA) * estimatedBurstTicks + ALPHA * t);
            }

            stats->totalBurstTicks += t;

            if (t > stats->maxBurstTicks)
                stats->maxBurstTicks = t;

            if (t < stats->minBurstTicks)
                stats->minBurstTicks = t;

            stats->numTotalBursts++;
        }
    }
    else if (status == BLOCKED) {
        printf("\nPID %d: BLOCKED to %s at Tick %d\n", pid, (st == RUNNING) ? "RUNNING" : "READY", stats->totalTicks);

        SetCpuCount(0);
    }
    lastWaitStartTicks = stats->totalTicks;
    lastBurstStartTicks = stats->totalTicks;

    status = st;
}
/* ======================= CUSTOM ======================= */
