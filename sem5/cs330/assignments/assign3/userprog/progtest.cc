// progtest.cc
//	Test routines for demonstrating that Nachos can load
//	a user program and execute it.
//
//	Also, routines for testing the Console hardware device.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "console.h"
#include "addrspace.h"
#include "synch.h"
#include "filesys.h"

void BatchStartFunction(int dummy)
{
    currentThread->Startup();
    machine->Run();
}

//----------------------------------------------------------------------
// LaunchUserProcess
// 	Run a user program.  Open the executable, load it into
//	memory, and jump to it.
//----------------------------------------------------------------------

void LaunchUserProcess(char *filename)
{
    OpenFile *executable = fileSystem->Open(filename);
    ProcessAddressSpace *space;

    if (executable == NULL)
    {
        printf("Unable to open file %s\n", filename);
        return;
    }

    space = new ProcessAddressSpace(executable, filename);
    currentThread->space = space;

    /* ------------------------ CUSTOM ------------------------ */
    currentThread->space->fileName = filename;
    /* ------------------------ CUSTOM ------------------------ */

    delete executable; // close file

    space->InitUserModeCPURegisters(); // set the initial register values
    space->RestoreContextOnSwitch();   // load page table register
    //printf("\nzzzzzzzz3\n");
    machine->Run(); // jump to the user progam
    ASSERT(FALSE);  // machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
}

// Data structures needed for the console test.  Threads making
// I/O requests wait on a Semaphore to delay until the I/O completes.

static Console *console;
static Semaphore *readAvail;
static Semaphore *writeDone;

//----------------------------------------------------------------------
// ConsoleInterruptHandlers
// 	Wake up the thread that requested the I/O.
//----------------------------------------------------------------------

static void ReadAvail(int arg) { readAvail->V(); }

static void WriteDone(int arg) { writeDone->V(); }

//----------------------------------------------------------------------
// ConsoleTest
// 	Test the console by echoing characters typed at the input onto
//	the output.  Stop when the user types a 'q'.
//----------------------------------------------------------------------

void ConsoleTest(char *in, char *out)
{
    char ch;

    console = new Console(in, out, ReadAvail, WriteDone, 0);
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);

    for (;;)
    {
        readAvail->P(); // wait for character to arrive
        ch = console->GetChar();
        console->PutChar(ch); // echo it!
        writeDone->P();       // wait for write to finish
        if (ch == 'q')
            return; // if q, quit
    }
}

//--------------------------------------------------------------------------------------------------
// ReadInputAndFork (multiprogramming test)
//	Read the scheduling algorithm.
//      Read a set of user programs along with the priorities.  Open the executables, load them into
//      memory, and invoke the scheduler.
//---------------------------------------------------------------------------------------------------

void ReadInputAndFork(char *filename)
{
    OpenFile *inFile = fileSystem->Open(filename);
    char c, buffer[16];
    unsigned batchSize = 0, bytesRead, charPointer, i;

    excludeMainThread = TRUE;

    if (inFile == NULL)
    {
        printf("Unable to open file %s\n", filename);
        return;
    }

    inFile->Read(&c, 1);
    schedulingAlgo = 0;
    // Read scheduling algorithm
    while (c != '\n')
    {
        schedulingAlgo = 10 * schedulingAlgo + c - '0';
        inFile->Read(&c, 1);
    }

    //printf("%d\n", schedulingAlgo);

    if ((schedulingAlgo == ROUND_ROBIN) || (schedulingAlgo == UNIX_SCHED))
    {
        ASSERT(SCHED_QUANTUM > 0);
    }

    bytesRead = inFile->Read(&c, 1);
    while (bytesRead != 0)
    {
        charPointer = 0;
        while ((c != ' ') && (c != '\n'))
        {
            batchProcesses[batchSize][charPointer] = c;
            charPointer++;
            bytesRead = inFile->Read(&c, 1);
        }
        batchProcesses[batchSize][charPointer] = '\0';
        if (c == '\n')
        {
            priority[batchSize] = MAX_NICE_PRIORITY;
        }
        else
        {
            bytesRead = inFile->Read(&c, 1);
            priority[batchSize] = 0;
            while (c != '\n')
            {
                priority[batchSize] = 10 * priority[batchSize] + c - '0';
                bytesRead = inFile->Read(&c, 1);
            }
        }
        //printf("%s %d\n", batchProcesses[batchSize], priority[batchSize]);
        batchSize++;
        bytesRead = inFile->Read(&c, 1);
    }
    delete inFile;

    for (i = 0; i < batchSize; i++)
    {
        // Create one child per iteration
        inFile = fileSystem->Open(batchProcesses[i]);
        if (inFile == NULL)
        {
            printf("Unable to open file %s\n", batchProcesses[i]);
            return;
        }
        sprintf(buffer, "Thread_%d", i + 1);
        NachOSThread *child = new NachOSThread(buffer, priority[i]);
        child->space = new ProcessAddressSpace(inFile, batchProcesses[i]);
        delete inFile;
        child->space->InitUserModeCPURegisters(); // set the initial register values
        child->SaveUserState();
        child->CreateThreadStack(BatchStartFunction, 0);
        child->Schedule();
        //printf("Created %d\n", i);
    }

    // Cleanly exit current thread
    // Assume exit code zero
    printf("[pid %d]: Exit called. Code: %d\n", currentThread->GetPID(), 0);
    exitThreadArray[currentThread->GetPID()] = true;

    // Find out if all threads have called exit
    for (i = 0; i < thread_index; i++)
    {
        if (!exitThreadArray[i])
            break;
    }
    currentThread->Exit(i == thread_index, 0);
}
