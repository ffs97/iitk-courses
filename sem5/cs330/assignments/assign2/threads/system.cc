// system.cc 
//	Nachos initialization and cleanup routines.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"

// This defines *all* of the global data structures used by Nachos.
// These are all initialized and de-allocated by this file.

NachOSThread *currentThread;            // the thread we are running now
NachOSThread *threadToBeDestroyed;        // the thread that just finished
ProcessScheduler *scheduler;            // the ready list
Interrupt *interrupt;            // interrupt status
Statistics *stats;            // performance metrics
Timer *timer;                // the hardware timer device,
// for invoking context switches

/* ----------------------- CUSTOM ----------------------- */
int numThreadsCreated = 0;
int numThreadsCurrent = 0;

int numTotalPages = 0;

List *listOfBlockedThreads = new List;
/* ----------------------- CUSTOM ----------------------- */

/* ======================= CUSTOM ======================= */
SchedulerType schedulerType = UNIX_4;

int TimerTicks = 100;
int averageTimerTicks = 132;
int idealTimerTicks = 20;

int minimumBasePriority = 50;

float ALPHA = 0.5;
/* ======================= CUSTOM ======================= */

#ifdef FILESYS_NEEDED
FileSystem *fileSystem;
#endif

#ifdef FILESYS
SynchDisk *synchDisk;
#endif

#ifdef USER_PROGRAM    // requires either FILESYS or FILESYS_STUB
Machine *machine;    // user program memory and registers
#endif

#ifdef NETWORK
PostOffice *postOffice;
#endif

bool initializedConsoleSemaphores;

// External definition, to allow us to take a pointer to this function
extern void Cleanup();


//----------------------------------------------------------------------
// TimerInterruptHandler
// 	Interrupt handler for the timer device.  The timer device is
//	set up to interrupt the CPU periodically (once every TimerTicks).
//	This routine is called each time there is a timer interrupt,
//	with interrupts disabled.
//
//	Note that instead of calling YieldCPU() directly (which would
//	suspend the interrupt handler, not the interrupted thread
//	which is what we wanted to context switch), we set a flag
//	so that once the interrupt handler is done, it will appear as 
//	if the interrupted thread called YieldCPU at the point it is 
//	was interrupted.
//
//	"dummy" is because every interrupt handler takes one argument,
//		whether it needs it or not.
//----------------------------------------------------------------------


/* ----------------------- CUSTOM ----------------------- */
void
wakeSleepingThreads() {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    NachOSThread *currentWaitingThread;
    int currentWaitingThreadWakeTime;

    while (!listOfBlockedThreads->IsEmpty()) {
        currentWaitingThread = (NachOSThread *) listOfBlockedThreads->SortedRemove(&currentWaitingThreadWakeTime);
        if (currentWaitingThreadWakeTime > stats->totalTicks) {
            listOfBlockedThreads->SortedInsert(currentWaitingThread, currentWaitingThreadWakeTime);
            break;
        }
        else {
            scheduler->MoveThreadToReadyQueue(currentWaitingThread);
        }
    }

    (void) interrupt->SetLevel(oldLevel);
}
/* ----------------------- CUSTOM ----------------------- */


/* ======================= CUSTOM ======================= */
void UpdatePriority(int arg) {
    NachOSThread *thread = (NachOSThread *) arg;
    thread->SetCpuCount(thread->GetCpuCount() >> 1);
}
/* ======================= CUSTOM ======================= */


static void
TimerInterruptHandler(int dummy) {
    if (interrupt->getStatus() != IdleMode) {
        /* ----------------------- CUSTOM ----------------------- */
        wakeSleepingThreads();
        /* ----------------------- CUSTOM ----------------------- */

        /* ======================= CUSTOM ======================= */
        printf("CURRENT RUNNING THREAD: ");
        if (currentThread != NULL) {
            printf("%d\n", currentThread->GetPID());
        }
        else {
            printf("NULL\n");
        }

        printf("TOTAL TICKS: %d, LAST BURST: %d\n", stats->totalTicks, currentThread->GetLastBurstStartTicks());
        if (schedulerType != NP_DEFAULT && schedulerType != SHORTEST_BURST &&
            stats->totalTicks - currentThread->GetLastBurstStartTicks() >= timer->TimeOfNextInterrupt()) {
            interrupt->YieldOnReturn();
        }
        /* ======================= CUSTOM ======================= */
    }
}


//----------------------------------------------------------------------
// Initialize
// 	Initialize Nachos global data structures.  Interpret command
//	line arguments in order to determine flags for the initialization.  
// 
//	"argc" is the number of command line arguments (including the name
//		of the command) -- ex: "nachos -d +" -> argc = 3 
//	"argv" is an array of strings, one for each command line argument
//		ex: "nachos -d +" -> argv = {"nachos", "-d", "+"}
//----------------------------------------------------------------------
void
Initialize(int argc, char **argv) {
    int argCount;
    char *debugArgs = "";
    bool randomYield = FALSE;

    initializedConsoleSemaphores = false;

#ifdef USER_PROGRAM
    bool debugUserProg = FALSE;    // single step user program
#endif
#ifdef FILESYS_NEEDED
    bool format = FALSE;    // format disk
#endif
#ifdef NETWORK
    double rely = 1;        // network reliability
    int netname = 0;        // UNIX socket name
#endif

    for (argc--, argv++; argc > 0; argc -= argCount, argv += argCount) {
        argCount = 1;
        if (!strcmp(*argv, "-d")) {
            if (argc == 1)
                debugArgs = "+";    // turn on all debug flags
            else {
                debugArgs = *(argv + 1);
                argCount = 2;
            }
        }
        else if (!strcmp(*argv, "-rs")) {
            ASSERT(argc > 1);
            RandomInit(atoi(*(argv + 1)));    // initialize pseudo-random
            // number generator
            randomYield = TRUE;
            argCount = 2;
        }
#ifdef USER_PROGRAM
        if (!strcmp(*argv, "-s"))
            debugUserProg = TRUE;
#endif
#ifdef FILESYS_NEEDED
        if (!strcmp(*argv, "-f"))
            format = TRUE;
#endif
#ifdef NETWORK
        if (!strcmp(*argv, "-l")) {
            ASSERT(argc > 1);
            rely = atof(*(argv + 1));
            argCount = 2;
        }
        else if (!strcmp(*argv, "-m")) {
            ASSERT(argc > 1);
            netname = atoi(*(argv + 1));
            argCount = 2;
        }
#endif
    }

    DebugInit(debugArgs);            // initialize DEBUG messages
    stats = new Statistics();            // collect statistics
    interrupt = new Interrupt;            // start up interrupt handling
    scheduler = new ProcessScheduler();        // initialize the ready queue

    /* ======================= CUSTOM ======================= */
    timer = new Timer(TimerInterruptHandler, 0, schedulerType == P_DEFAULT);
    /* ======================= CUSTOM ======================= */

    threadToBeDestroyed = NULL;

    // We didn't explicitly allocate the current thread we are running in.
    // But if it ever tries to give up the CPU, we better have a Thread
    // object to save its state. 
    currentThread = new NachOSThread("main");
    currentThread->setStatus(RUNNING);

    interrupt->Enable();
    CallOnUserAbort(Cleanup);            // if user hits ctl-C

#ifdef USER_PROGRAM
    machine = new Machine(debugUserProg);    // this must come first
#endif

#ifdef FILESYS
    synchDisk = new SynchDisk("DISK");
#endif

#ifdef FILESYS_NEEDED
    fileSystem = new FileSystem(format);
#endif

#ifdef NETWORK
    postOffice = new PostOffice(netname, rely, 10);
#endif
}


//----------------------------------------------------------------------
// Cleanup
// 	Nachos is halting.  De-allocate global data structures.
//----------------------------------------------------------------------
void
Cleanup() {
    printf("\nCleaning up...\n");
#ifdef NETWORK
    delete postOffice;
#endif

#ifdef USER_PROGRAM
    delete machine;
#endif

#ifdef FILESYS_NEEDED
    delete fileSystem;
#endif

#ifdef FILESYS
    delete synchDisk;
#endif

    delete timer;
    delete scheduler;
    delete interrupt;

    Exit(0);
}

