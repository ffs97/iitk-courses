#include "copyright.h"
#include "machine.h"
#include "system.h"

static char *exceptionNames[] = {"no exception", "syscall",
                                 "page fault/no TLB entry", "page read only",
                                 "bus error", "address error", "overflow",
                                 "illegal instruction"};

static void CheckEndian()
{
    union checkit {
        char charword[4];
        unsigned int intword;
    } check;

    check.charword[0] = 1;
    check.charword[1] = 2;
    check.charword[2] = 3;
    check.charword[3] = 4;

#ifdef HOST_IS_BIG_ENDIAN
    ASSERT(check.intword == 0x01020304);
#else
    ASSERT(check.intword == 0x04030201);
#endif
}

Machine::Machine(bool debug)
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
        registers[i] = 0;
    mainMemory = new char[MemorySize];
    for (i = 0; i < MemorySize; i++)
        mainMemory[i] = 0;

#ifdef USE_TLB
    tlb = new TranslationEntry[TLBSize];
    for (i = 0; i < TLBSize; i++)
        tlb[i].valid = FALSE;
    KernelPageTable = NULL;
#else
    tlb = NULL;
    KernelPageTable = NULL;
#endif

    singleStep = debug;
    CheckEndian();
}

Machine::~Machine()
{
    delete[] mainMemory;
    if (tlb != NULL)
        delete[] tlb;
}

void Machine::RaiseException(ExceptionType which, int badVAddr)
{
    DEBUG('m', "Exception: %s\n", exceptionNames[which]);

    registers[BadVAddrReg] = badVAddr;
    DelayedLoad(0, 0);
    interrupt->setStatus(SystemMode);
    ExceptionHandler(which);
    interrupt->setStatus(UserMode);
}

void Machine::Debugger()
{
    char *buf = new char[80];
    int num;

    interrupt->DumpState();
    DumpState();
    printf("%d> ", stats->totalTicks);
    fflush(stdout);
    fgets(buf, 80, stdin);
    if (sscanf(buf, "%d", &num) == 1)
        runUntilTime = num;
    else
    {
        runUntilTime = 0;
        switch (*buf)
        {
        case '\n':
            break;

        case 'c':
            singleStep = FALSE;
            break;

        case '?':
            printf("Machine commands:\n");
            printf("    <return>  execute one instruction\n");
            printf("    <number>  run until the given timer tick\n");
            printf("    c         run until completion\n");
            printf("    ?         print help message\n");
            break;
        }
    }
    delete[] buf;
}

void Machine::DumpState()
{
    int i;

    printf("Machine registers:\n");
    for (i = 0; i < NumGPRegs; i++)
        switch (i)
        {
        case StackReg:
            printf("\tSP(%d):\t0x%x%s", i, registers[i],
                   ((i % 4) == 3) ? "\n" : "");
            break;

        case RetAddrReg:
            printf("\tRA(%d):\t0x%x%s", i, registers[i],
                   ((i % 4) == 3) ? "\n" : "");
            break;

        default:
            printf("\t%d:\t0x%x%s", i, registers[i],
                   ((i % 4) == 3) ? "\n" : "");
            break;
        }

    printf("\tHi:\t0x%x", registers[HiReg]);
    printf("\tLo:\t0x%x\n", registers[LoReg]);
    printf("\tPC:\t0x%x", registers[PCReg]);
    printf("\tNextPC:\t0x%x", registers[NextPCReg]);
    printf("\tPrevPC:\t0x%x\n", registers[PrevPCReg]);
    printf("\tLoad:\t0x%x", registers[LoadReg]);
    printf("\tLoadV:\t0x%x\n", registers[LoadValueReg]);
    printf("\n");
}

int Machine::ReadRegister(int num)
{
    ASSERT((num >= 0) && (num < NumTotalRegs));
    return registers[num];
}

void Machine::WriteRegister(int num, int value)
{
    ASSERT((num >= 0) && (num < NumTotalRegs));

    registers[num] = value;
}
