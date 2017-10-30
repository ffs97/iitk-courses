// system.h
//	All global variables used in Nachos are defined here.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef SYSTEM_H
#define SYSTEM_H

#include "copyright.h"
#include "utility.h"
#include "thread.h"
#include "scheduler.h"
#include "interrupt.h"
#include "stats.h"
#include "timer.h"


// Initialization and cleanup routines
extern void Initialize(int argc, char **argv);    // Initialization,
// called before anything else
extern void Cleanup();                // Cleanup, called when
// Nachos is done.

extern NachOSThread *currentThread;            // the thread holding the CPU
extern NachOSThread *threadToBeDestroyed;        // the thread that just finished
extern ProcessScheduler *scheduler;            // the thread scheduler
extern Interrupt *interrupt;            // interrupt status
extern Statistics *stats;            // performance metrics
extern Timer *timer;                // the hardware alarm clock

extern bool initializedConsoleSemaphores;       // Used to initialize the semaphores for console I/O exactly once

/* ----------------------- CUSTOM ----------------------- */
extern int numThreadsCreated;
extern int numThreadsCurrent;

extern int numTotalPages;

extern List *listOfBlockedThreads;
/* ----------------------- CUSTOM ----------------------- */

/* ======================= CUSTOM ======================= */
enum SchedulerType {
    P_DEFAULT,
    NP_DEFAULT,
    SHORTEST_BURST,
    ROUND_ROBIN_1,
    ROUND_ROBIN_2,
    ROUND_ROBIN_3,
    ROUND_ROBIN_4,
    UNIX_1,
    UNIX_2,
    UNIX_3,
    UNIX_4
};

extern SchedulerType schedulerType;

extern int TimerTicks;
extern int averageTimerTicks;
extern int idealTimerTicks;

extern int minimumBasePriority;

extern float ALPHA;

extern void UpdatePriority(int arg);
/* ======================= CUSTOM ======================= */

#ifdef USER_PROGRAM
#include "machine.h"


extern Machine *machine;    // user program memory and registers
#endif

#ifdef FILESYS_NEEDED        // FILESYS or FILESYS_STUB
#include "filesys.h"


extern FileSystem *fileSystem;
#endif

#ifdef FILESYS
#include "synchdisk.h"


extern SynchDisk *synchDisk;
#endif

#ifdef NETWORK
#include "post.h"


extern PostOffice *postOffice;
#endif

#endif // SYSTEM_H
