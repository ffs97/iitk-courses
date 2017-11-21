#ifndef STATS_H
#define STATS_H

#include "copyright.h"

class Statistics
{
public:
  int totalTicks;
  int idleTicks;
  int systemTicks;
  int userTicks;

  int start_time;
  int total_wait_time;
  int cpu_time;
  int max_cpu_burst;
  int min_cpu_burst;
  int cpu_burst_count;
  int empty_ready_queue_time;

  int preemptive_switch;
  int nonpreemptive_switch;

  int numTotalThreads;

  int burstEstimateError;

  int numDiskReads;
  int numDiskWrites;
  int numConsoleCharsRead;
  int numConsoleCharsWritten;
  int numPageFaults;
  int numPacketsSent;
  int numPacketsRecvd;

  Statistics();

  void Print();
};

#define UserTick 1
#define SystemTick 10
#define RotationTime 500
#define SeekTime 500
#define ConsoleTime 100
#define NetworkTime 100
#define TimerTicks 100

#endif
