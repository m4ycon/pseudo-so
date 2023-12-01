#pragma once
#include "../common.h"
#include "../process/process.h"

class Scheduler
{
public:
  Scheduler(TimePoint startTime);

  void addReadyProcess(Process *process);
  bool isEmpty();
  Process *getNextProcess();

private:

  queue<Process *> readyQueue, realtimeQueue;
  queue<Process *> userQueue1, userQueue2, userQueue3;

  chrono::steady_clock::time_point startTime;

  std::mutex schedulerMutex;

  void addProcess(Process *process);
};
