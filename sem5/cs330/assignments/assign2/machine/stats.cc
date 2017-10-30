// stats.h 
//	Routines for managing statistics about Nachos performance.
//
// DO NOT CHANGE -- these stats are maintained by the machine emulation.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "utility.h"
#include "stats.h"
/* ======================= CUSTOM ======================= */
#include "system.h"
/* ======================= CUSTOM ======================= */

//----------------------------------------------------------------------
// Statistics::Statistics
// 	Initialize performance metrics to zero, at system startup.
//----------------------------------------------------------------------

Statistics::Statistics() {
    totalTicks = idleTicks = systemTicks = userTicks = 0;
    numDiskReads = numDiskWrites = 0;
    numConsoleCharsRead = numConsoleCharsWritten = 0;
    numPageFaults = numPacketsSent = numPacketsRecvd = 0;
    /* ======================= CUSTOM ======================= */
    totalBurstTicks = totalWaitTicks = 0;

    minBurstTicks = 1000000;
    maxBurstTicks = 0;
    numTotalBursts = 0;

    minCompletionTime = 1000000;
    maxCompletionTime = 0;

    averageCompletionTime = 0;
    averageSquareCompletionTime = 0;
    /* ======================= CUSTOM ======================= */
}

//----------------------------------------------------------------------
// Statistics::Print
// 	Print performance metrics, when we've finished everything
//	at system shutdown.
//----------------------------------------------------------------------

void
Statistics::Print() {
    printf("Ticks: total %d, idle %d, system %d, user %d\n", totalTicks, idleTicks, systemTicks, userTicks);
    printf("Disk I/O: reads %d, writes %d\n", numDiskReads, numDiskWrites);
    printf("Console I/O: reads %d, writes %d\n", numConsoleCharsRead, numConsoleCharsWritten);
    printf("Paging: faults %d\n", numPageFaults);
    printf("Network I/O: packets received %d, sent %d\n", numPacketsRecvd, numPacketsSent);
    /* ======================= CUSTOM ======================= */
    printf("Ticks spent in CPU Burst: %d\n", totalBurstTicks);
    printf("Ticks spent waiting in Queue: %d\n", totalWaitTicks);
    printf("\tCPU Utilization: %.2f\n", 100.0 * totalBurstTicks / totalTicks);
    printf("Number of non-zero CPU Bursts: %d\n", numTotalBursts);
    printf("Average Ticks in CPU Burst: %d\n",
           (int) (((float) totalBurstTicks) / numTotalBursts + 0.5));
    printf("Minimum CPU Burst Ticks: %d\n", minBurstTicks);
    printf("Maximum CPU Burst Ticks: %d\n", maxBurstTicks);
    printf("Minimum Thread Completion Time: %d\n", minCompletionTime);
    printf("Maximum Thread Completion Time: %d\n", maxCompletionTime);
    printf("Average Thread Completion Time: %lf\n", averageCompletionTime);
    printf("Variance of Thread Completion Times: %lf\n",
           averageSquareCompletionTime - averageCompletionTime * averageCompletionTime);
    /* ======================= CUSTOM ======================= */
}
