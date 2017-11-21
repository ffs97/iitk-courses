#ifndef SYSTEM_H
#define SYSTEM_H

#include "copyright.h"
#include "utility.h"
#include "thread.h"
#include "scheduler.h"
#include "interrupt.h"
#include "stats.h"
#include "timer.h"
#include "disk.h"

#define MAX_THREAD_COUNT 1000
#define MAX_BATCH_SIZE 100

#define NON_PREEMPTIVE_BASE 1
#define NON_PREEMPTIVE_SJF 2
#define ROUND_ROBIN 3
#define UNIX_SCHED 4

#define SCHED_QUANTUM 100

#define INITIAL_TAU SystemTick
#define ALPHA 0.5

#define MAX_NICE_PRIORITY 100
#define MIN_NICE_PRIORITY 0
#define DEFAULT_BASE_PRIORITY 50
#define GET_NICE_FROM_PARENT -1

extern void Initialize(int argc, char **argv);

extern void Cleanup();

extern NachOSThread *currentThread;
extern NachOSThread *threadToBeDestroyed;
extern ProcessScheduler *scheduler;
extern Interrupt *interrupt;
extern Statistics *stats;
extern Timer *timer;
extern unsigned numPagesAllocated;
extern int schedulingAlgo;

extern NachOSThread *threadArray[];
extern unsigned thread_index;
extern bool initializedConsoleSemaphores;
extern bool exitThreadArray[];

extern char **batchProcesses;
extern int *priority;

extern int cpu_burst_start_time;
extern int completionTimeArray[];
extern bool excludeMainThread;

/* ------------------------ CUSTOM ------------------------ */
#define INF 10000007

class PhysicalPagesList
{
public:
  PhysicalPagesList()
  {
    virtualPage = -1;
    shared = FALSE;
    threadPID = -1;
    reference = FALSE;
    usageTime = INF;
    entryTime = INF;
  }

  int virtualPage;
  int threadPID;
  bool reference;
  bool shared;
  int usageTime;
  int entryTime;
};

extern int pageReplaceAlgo;

extern int NumPhysPages;
extern int MemorySize;

extern PhysicalPagesList *physicalPagesList;

extern unsigned numSharedPages;

#define PageSize SectorSize

#define NDEMAND 0
#define RANDOM 1
#define FIFO 2
#define LRU 3
#define LRUCLOCK 4

extern int pointReference;
/* ------------------------ CUSTOM ------------------------ */

class TimeSortedWaitQueue
{
private:
  NachOSThread *t;
  unsigned when;
  TimeSortedWaitQueue *next;

public:
  TimeSortedWaitQueue(NachOSThread *th, unsigned w)
  {
    t = th;
    when = w;
    next = NULL;
  }

  ~TimeSortedWaitQueue(void) {}

  NachOSThread *GetThread(void) { return t; }

  unsigned GetWhen(void) { return when; }

  TimeSortedWaitQueue *GetNext(void) { return next; }

  void SetNext(TimeSortedWaitQueue *n) { next = n; }
};

extern TimeSortedWaitQueue *sleepQueueHead;

#ifdef USER_PROGRAM
#include "machine.h"
extern Machine *machine;
#endif

#ifdef FILESYS_NEEDED
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

#endif
