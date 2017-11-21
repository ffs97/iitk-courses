#include "copyright.h"
#include "system.h"

NachOSThread *currentThread;
NachOSThread *threadToBeDestroyed;
ProcessScheduler *scheduler;
Interrupt *interrupt;
Statistics *stats;
Timer *timer;

unsigned numPagesAllocated;

NachOSThread *threadArray[MAX_THREAD_COUNT];
unsigned thread_index;
bool initializedConsoleSemaphores;
bool exitThreadArray[MAX_THREAD_COUNT];

TimeSortedWaitQueue *sleepQueueHead;

int schedulingAlgo;
char **batchProcesses;
int *priority;

int cpu_burst_start_time;
int completionTimeArray[MAX_THREAD_COUNT];
bool excludeMainThread;

#ifdef FILESYS_NEEDED
FileSystem *fileSystem;
#endif

#ifdef FILESYS
SynchDisk *synchDisk;
#endif

#ifdef USER_PROGRAM
Machine *machine;
#endif

#ifdef NETWORK
PostOffice *postOffice;
#endif

extern void Cleanup();

/* ------------------------ CUSTOM ------------------------ */
unsigned numSharedPages;

int pageReplaceAlgo;

int NumPhysPages;
int MemorySize;

PhysicalPagesList *physicalPagesList;

int pointReference;
/* ------------------------ CUSTOM ------------------------ */

static void
TimerInterruptHandler(int dummy)
{
    TimeSortedWaitQueue *ptr;
    if (interrupt->getStatus() != IdleMode)
    {

        while ((sleepQueueHead != NULL) && (sleepQueueHead->GetWhen() <= (unsigned)stats->totalTicks))
        {
            sleepQueueHead->GetThread()->Schedule();
            ptr = sleepQueueHead;
            sleepQueueHead = sleepQueueHead->GetNext();
            delete ptr;
        }

        if ((schedulingAlgo == ROUND_ROBIN) || (schedulingAlgo == UNIX_SCHED))
        {
            if ((stats->totalTicks - cpu_burst_start_time) >= SCHED_QUANTUM)
            {
                ASSERT(cpu_burst_start_time == currentThread->GetCPUBurstStartTime());
                interrupt->YieldOnReturn();
            }
        }
    }
}

void Initialize(int argc, char **argv)
{
    int argCount, i;
    char *debugArgs = "";
    bool randomYield = FALSE;

    initializedConsoleSemaphores = false;
    numPagesAllocated = 0;

    schedulingAlgo = NON_PREEMPTIVE_BASE;

    /* ------------------------ CUSTOM ------------------------ */
    numSharedPages = 0;

    switch (pageReplaceAlgo)
    {
    case 1:
        NumPhysPages = 13;
        break;
    case 2:
        NumPhysPages = 10;
        break;
    case 3:
        NumPhysPages = 12;
        break;
    default:
        NumPhysPages = 1024;
    }

    MemorySize = NumPhysPages * PageSize;

    physicalPagesList = new PhysicalPagesList[NumPhysPages];

    pointReference = 0;
    /* ------------------------ CUSTOM ------------------------ */

    batchProcesses = new char *[MAX_BATCH_SIZE];
    ASSERT(batchProcesses != NULL);
    for (i = 0; i < MAX_BATCH_SIZE; i++)
    {
        batchProcesses[i] = new char[256];
        ASSERT(batchProcesses[i] != NULL);
    }

    priority = new int[MAX_BATCH_SIZE];
    ASSERT(priority != NULL);

    excludeMainThread = FALSE;

    for (i = 0; i < MAX_THREAD_COUNT; i++)
    {
        threadArray[i] = NULL;
        exitThreadArray[i] = false;
        completionTimeArray[i] = -1;
    }
    thread_index = 0;

    sleepQueueHead = NULL;

#ifdef USER_PROGRAM
    bool debugUserProg = FALSE;
#endif
#ifdef FILESYS_NEEDED
    bool format = FALSE;
#endif
#ifdef NETWORK
    double rely = 1;
    int netname = 0;
#endif

    for (argc--, argv++; argc > 0; argc -= argCount, argv += argCount)
    {
        argCount = 1;
        if (!strcmp(*argv, "-d"))
        {
            if (argc == 1)
                debugArgs = "+";
            else
            {
                debugArgs = *(argv + 1);
                argCount = 2;
            }
        }
        else if (!strcmp(*argv, "-rs"))
        {
            ASSERT(argc > 1);
            RandomInit(atoi(*(argv + 1)));

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
        if (!strcmp(*argv, "-l"))
        {
            ASSERT(argc > 1);
            rely = atof(*(argv + 1));
            argCount = 2;
        }
        else if (!strcmp(*argv, "-m"))
        {
            ASSERT(argc > 1);
            netname = atoi(*(argv + 1));
            argCount = 2;
        }
#endif
    }

    DebugInit(debugArgs);
    stats = new Statistics();
    interrupt = new Interrupt;
    scheduler = new ProcessScheduler();

    timer = new Timer(TimerInterruptHandler, 0, randomYield);

    threadToBeDestroyed = NULL;

    currentThread = NULL;
    currentThread = new NachOSThread("main", MIN_NICE_PRIORITY);
    currentThread->setStatus(RUNNING);
    stats->start_time = stats->totalTicks;
    cpu_burst_start_time = stats->totalTicks;

    interrupt->Enable();
    CallOnUserAbort(Cleanup);

#ifdef USER_PROGRAM
    machine = new Machine(debugUserProg);
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

void Cleanup()
{
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
