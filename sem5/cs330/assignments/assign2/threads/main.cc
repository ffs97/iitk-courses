// main.cc 
//	Bootstrap code to initialize the operating system kernel.
//
//	Allows direct calls into internal operating system functions,
//	to simplify debugging and testing.  In practice, the
//	bootstrap code would just initialize data structures,
//	and start a user program to print the login prompt.
//
// 	Most of this file is not needed until later assignments.
//
// Usage: nachos -d <debugflags> -rs <random seed #>
//		-s -x <nachos file> -c <consoleIn> <consoleOut>
//		-f -cp <unix file> <nachos file>
//		-p <nachos file> -r <nachos file> -l -D -t
//              -n <network reliability> -m <machine id>
//              -o <other machine id>
//              -z
//
//    -d causes certain debugging messages to be printed (cf. utility.h)
//    -rs causes Yield to occur at random (but repeatable) spots
//    -z prints the copyright message
//
//  USER_PROGRAM
//    -s causes user programs to be executed in single-step mode
//    -x runs a user program
//    -c tests the console
//
//  FILESYS
//    -f causes the physical disk to be formatted
//    -cp copies a file from UNIX to Nachos
//    -p prints a Nachos file to stdout
//    -r removes a Nachos file from the file system
//    -l lists the contents of the Nachos directory
//    -D prints the contents of the entire file system 
//    -t tests the performance of the Nachos file system
//
//  NETWORK
//    -n sets the network reliability
//    -m sets this machine's host id (needed for the network)
//    -o runs a simple test of the Nachos network software
//
//  NOTE -- flags are ignored until the relevant assignment.
//  Some of the flags are interpreted here; some in system.cc.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#define MAIN
#include "copyright.h"


#undef MAIN

#include "utility.h"
#include "system.h"


// External functions used by this file

extern void ThreadTest(void), Copy(char *unixFile, char *nachosFile);

extern void Print(char *file), PerformanceTest(void);

extern void LaunchUserProcess(char *file), ConsoleTest(char *in, char *out);

extern void MailTest(int networkID);

/* ======================= CUSTOM ======================= */
extern void fork_init_func(int arg);


int
GetFileNameLength(char *c) {
    int l = 0;
    while (c[l] != ' ' && c[l] != '\t' && c[l] != '\0') {
        l++;
    }
    return l;
}


int
GetPriority(char *c) {
    int i = 0;
    while (c[i] == ' ' || c[i] == '\t') i++;

    if (c[i] == '-') return 0;

    int priority = 0;
    while (c[i] != '\0') {
        priority = priority * 10 + (c[i++] - '0');
    }

    return priority;
}
/* ======================= CUSTOM ======================= */


//----------------------------------------------------------------------
// main
// 	Bootstrap the operating system kernel.  
//	
//	Check command line arguments
//	Initialize data structures
//	(optionally) Call test procedure
//
//	"argc" is the number of command line arguments (including the name
//		of the command) -- ex: "nachos -d +" -> argc = 3 
//	"argv" is an array of strings, one for each command line argument
//		ex: "nachos -d +" -> argv = {"nachos", "-d", "+"}
//----------------------------------------------------------------------

int
main(int argc, char **argv) {
    int argCount;            // the number of arguments
    // for a particular command

    DEBUG('t', "Entering main");
    (void) Initialize(argc, argv);

#ifdef THREADS
    ThreadTest();
#endif

    for (argc--, argv++; argc > 0; argc -= argCount, argv += argCount) {
        argCount = 1;
        if (!strcmp(*argv, "-z"))               // print copyright
            printf(copyright);

#ifdef USER_PROGRAM
        if (!strcmp(*argv, "-x")) {            // run a user program
            ASSERT(argc > 1);
            LaunchUserProcess(*(argv + 1));
            argCount = 2;
        }
        else if (!strcmp(*argv, "-c")) {      // test the console
            if (argc == 1)
                ConsoleTest(NULL, NULL);
            else {
                ASSERT(argc > 2);
                ConsoleTest(*(argv + 1), *(argv + 2));
                argCount = 3;
            }
            interrupt->Halt();        // once we start the console, then
            // Nachos will loop forever waiting
            // for console input
        }
        /* ======================= CUSTOM ======================= */
        else if (!strcmp(*argv, "-F")) {
            IntStatus oldLevel = interrupt->SetLevel(IntOff);

            FILE *file;
            file = fopen(argv[1], "r");

            char line[1024];
            int priority, len, option;

            fscanf(file, "%d\n", &option);
            schedulerType = (SchedulerType) option;

            switch(schedulerType) {
                case UNIX_1:
                case ROUND_ROBIN_1:
                    TimerTicks = averageTimerTicks / 4;
                    break;
                case UNIX_2:
                case ROUND_ROBIN_2:
                    TimerTicks = averageTimerTicks / 2;
                    break;
                case UNIX_3:
                case ROUND_ROBIN_3:
                    TimerTicks = 3 * averageTimerTicks / 4;
                    break;
                case UNIX_4:
                case ROUND_ROBIN_4:
                    TimerTicks = (int) averageTimerTicks * idealTimerTicks;
                    break;
                default:
                    TimerTicks = 100;
            }

            while (fscanf(file, "%[^\n]\n", line) != EOF) {
                len = GetFileNameLength(line);
                priority = GetPriority(line + len);

                char filename[len + 1];
                strncpy(filename, line, len);
                filename[len] = '\0';

                OpenFile *executable = fileSystem->Open(filename);
                ProcessAddressSpace *space;

                if (executable == NULL) {
                    printf("Unable to open file %s\n", filename);
                    continue;
                }
                space = new ProcessAddressSpace(executable);

                delete executable;

                space->InitUserModeCPURegisters();
                space->RestoreContextOnSwitch();

                NachOSThread *newThread;
                if (schedulerType == UNIX_1 || schedulerType == UNIX_2 || schedulerType == UNIX_3 || schedulerType == UNIX_4) {
                    newThread = new NachOSThread(filename, minimumBasePriority + priority);
                }
                else {
                    newThread = new NachOSThread(filename);
                }
                newThread->space = space;
                newThread->SaveUserState();
                newThread->CreateThreadStack(fork_init_func, 0);

                scheduler->MoveThreadToReadyQueue(newThread);
            }
            currentThread->AddExitCode(0);
            fclose(file);

            (void) interrupt->SetLevel(oldLevel);
        }
        /* ======================= CUSTOM ======================= */
#endif // USER_PROGRAM
#ifdef FILESYS
        if (!strcmp(*argv, "-cp")) {        // copy from UNIX to Nachos
            ASSERT(argc > 2);
            Copy(*(argv + 1), *(argv + 2));
            argCount = 3;
        }
        else if (!strcmp(*argv, "-p")) {    // print a Nachos file
            ASSERT(argc > 1);
            Print(*(argv + 1));
            argCount = 2;
        }
        else if (!strcmp(*argv, "-r")) {    // remove Nachos file
            ASSERT(argc > 1);
            fileSystem->Remove(*(argv + 1));
            argCount = 2;
        }
        else if (!strcmp(*argv, "-l")) {    // list Nachos directory
            fileSystem->List();
        }
        else if (!strcmp(*argv, "-D")) {    // print entire filesystem
            fileSystem->Print();
        }
        else if (!strcmp(*argv, "-t")) {    // performance test
            PerformanceTest();
        }
#endif // FILESYS
#ifdef NETWORK
        if (!strcmp(*argv, "-o")) {
            ASSERT(argc > 1);
            Delay(2);                // delay for 2 seconds
            // to give the user time to
            // start up another nachos
            MailTest(atoi(*(argv + 1)));
            argCount = 2;
        }
#endif // NETWORK
    }

    currentThread->FinishThread();    // NOTE: if the procedure "main"
    // returns, then the program "nachos"
    // will exit (as any other normal program
    // would).  But there may be other
    // threads on the ready list.  We switch
    // to those threads by saying that the
    // "main" thread is finished, preventing
    // it from returning.
    return (0);            // Not reached...
}
