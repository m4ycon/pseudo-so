#pragma once
#include "./common.h"
#include "./process.h"


class Scheduler {
  public:
    Scheduler();

    void addProcess(Process* process);
    Process* removeProcess();


    queue<Process*> realtimeQueue;
    priority_queue<Process*> userQueue1, userQueue2, userQueue3;
  private:
};
