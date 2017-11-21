#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "console.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvail(int arg) { readAvail->V(); }

static void WriteDone(int arg) { writeDone->V(); }

extern void LaunchUserProcess(char *);

void ForkStartFunction(int dummy)
{
    currentThread->Startup();
    machine->Run();
}

static void ConvertIntToHex(unsigned v, Console *console)
{
    unsigned x;
    if (v == 0)
        return;
    ConvertIntToHex(v / 16, console);
    x = v % 16;
    if (x < 10)
    {
        writeDone->P();
        console->PutChar('0' + x);
    }
    else
    {
        writeDone->P();
        console->PutChar('a' + x - 10);
    }
}

void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    int memval, vaddr, printval, tempval, exp;
    unsigned printvalus;
    if (!initializedConsoleSemaphores)
    {
        readAvail = new Semaphore("read avail", 0);
        writeDone = new Semaphore("write done", 1);
        initializedConsoleSemaphores = true;
    }
    Console *console = new Console(NULL, NULL, ReadAvail, WriteDone, 0);
    int exitcode;
    unsigned i;
    char buffer[1024];
    int waitpid;
    int whichChild;
    NachOSThread *child;
    unsigned sleeptime;

    if ((which == SyscallException) && (type == SysCall_Halt))
    {
        DEBUG('a', "Shutdown, initiated by user program.\n");
        interrupt->Halt();
    }
    else if ((which == SyscallException) && (type == SysCall_Exit))
    {
        exitcode = machine->ReadRegister(4);
        printf("[pid %d]: Exit called. Code: %d\n", currentThread->GetPID(), exitcode);

        exitThreadArray[currentThread->GetPID()] = true;

        for (i = 0; i < thread_index; i++)
        {
            if (!exitThreadArray[i])
                break;
        }
        currentThread->Exit(i == thread_index, exitcode);
    }
    else if ((which == SyscallException) && (type == SysCall_Exec))
    {
        vaddr = machine->ReadRegister(4);

        bool read = FALSE;
        while (read == FALSE)
            read = machine->ReadMem(vaddr, 1, &memval);

        i = 0;
        while ((*(char *)&memval) != '\0')
        {
            buffer[i] = (*(char *)&memval);
            i++;
            vaddr++;
            bool read = FALSE;
            while (read == FALSE)
                read = machine->ReadMem(vaddr, 1, &memval);
        }
        buffer[i] = (*(char *)&memval);
        LaunchUserProcess(buffer);
    }
    else if ((which == SyscallException) && (type == SysCall_Join))
    {
        waitpid = machine->ReadRegister(4);

        whichChild = currentThread->CheckIfChild(waitpid);
        if (whichChild == -1)
        {
            printf("[pid %d] Cannot join with non-existent child [pid %d].\n", currentThread->GetPID(), waitpid);
            machine->WriteRegister(2, -1);

            machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
            machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
            machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
        }
        else
        {
            exitcode = currentThread->JoinWithChild(whichChild);
            machine->WriteRegister(2, exitcode);

            machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
            machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
            machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
        }
    }
    /* ------------------------ CUSTOM ------------------------ */
    else if ((which == PageFaultException))
    {
        ASSERT(pageReplaceAlgo != 0);

        IntStatus oldLevel = interrupt->SetLevel(IntOff);

        int vpaddress = machine->ReadRegister(BadVAddrReg);
        currentThread->space->DemandAllocation(vpaddress);

        currentThread->SortedInsertInWaitQueue(1000 + stats->totalTicks);

        (void)interrupt->SetLevel(oldLevel);
    }
    else if ((which == SyscallException) && (type == SysCall_ShmAllocate))
    {
        unsigned int size = machine->ReadRegister(4);
        unsigned int startingVirtualAddress = currentThread->space->AllocateSharedMemory(size);

        machine->WriteRegister(2, startingVirtualAddress);

        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
    }
    else if ((which == SyscallException) && (type == SysCall_Fork))
    {

        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);

        IntStatus oldLevel = interrupt->SetLevel(IntOff);

        child = new NachOSThread("Forked thread", GET_NICE_FROM_PARENT);

        child->space = new ProcessAddressSpace(currentThread->space);
        child->space->InitiateForkedProcessAddressSpace(currentThread->space, child->GetPID());
        child->SaveUserState();
        child->ResetReturnValue();
        child->CreateThreadStack(ForkStartFunction, 0);
        child->Schedule();

        machine->WriteRegister(2, child->GetPID());

        (void)interrupt->SetLevel(oldLevel);
    }
    /* ------------------------ CUSTOM ------------------------ */
    else if ((which == SyscallException) && (type == SysCall_Yield))
    {
        currentThread->YieldCPU();

        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
    }
    else if ((which == SyscallException) && (type == SysCall_PrintInt))
    {
        printval = machine->ReadRegister(4);
        if (printval == 0)
        {
            writeDone->P();
            console->PutChar('0');
        }
        else
        {
            if (printval < 0)
            {
                writeDone->P();
                console->PutChar('-');
                printval = -printval;
            }
            tempval = printval;
            exp = 1;
            while (tempval != 0)
            {
                tempval = tempval / 10;
                exp = exp * 10;
            }
            exp = exp / 10;
            while (exp > 0)
            {
                writeDone->P();
                console->PutChar('0' + (printval / exp));
                printval = printval % exp;
                exp = exp / 10;
            }
        }

        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
    }
    else if ((which == SyscallException) && (type == SysCall_PrintChar))
    {
        writeDone->P();
        console->PutChar(machine->ReadRegister(4));

        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
    }
    else if ((which == SyscallException) && (type == SysCall_PrintString))
    {
        vaddr = machine->ReadRegister(4);
        bool read = FALSE;
        while (read == FALSE)
            read = machine->ReadMem(vaddr, 1, &memval);
        while ((*(char *)&memval) != '\0')
        {
            writeDone->P();
            console->PutChar(*(char *)&memval);
            vaddr++;
            bool read = FALSE;
            while (read == FALSE)
                read = machine->ReadMem(vaddr, 1, &memval);
        }

        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
    }
    else if ((which == SyscallException) && (type == SysCall_GetReg))
    {
        machine->WriteRegister(2, machine->ReadRegister(machine->ReadRegister(4)));

        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
    }
    else if ((which == SyscallException) && (type == SysCall_GetPA))
    {
        vaddr = machine->ReadRegister(4);
        machine->WriteRegister(2, machine->GetPA(vaddr));

        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
    }
    else if ((which == SyscallException) && (type == SysCall_GetPID))
    {
        machine->WriteRegister(2, currentThread->GetPID());

        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
    }
    else if ((which == SyscallException) && (type == SysCall_GetPPID))
    {
        machine->WriteRegister(2, currentThread->GetPPID());

        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
    }
    else if ((which == SyscallException) && (type == SysCall_Sleep))
    {
        sleeptime = machine->ReadRegister(4);
        if (sleeptime == 0)
        {
            currentThread->YieldCPU();
        }
        else
        {
            currentThread->SortedInsertInWaitQueue(sleeptime + stats->totalTicks);
        }

        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
    }
    else if ((which == SyscallException) && (type == SysCall_Time))
    {
        machine->WriteRegister(2, stats->totalTicks);

        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
    }
    else if ((which == SyscallException) && (type == SysCall_PrintIntHex))
    {
        printvalus = (unsigned)machine->ReadRegister(4);
        writeDone->P();
        console->PutChar('0');
        writeDone->P();
        console->PutChar('x');
        if (printvalus == 0)
        {
            writeDone->P();
            console->PutChar('0');
        }
        else
        {
            ConvertIntToHex(printvalus, console);
        }

        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
    }
    else if ((which == SyscallException) && (type == SysCall_NumInstr))
    {
        machine->WriteRegister(2, currentThread->GetInstructionCount());

        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
    }
    else
    {
        printf("Unexpected user mode exception %d %d\n", which, type);
        ASSERT(FALSE);
    }
}
