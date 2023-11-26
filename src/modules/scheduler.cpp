#include "../include/scheduler.h"

Scheduler::Scheduler()
{
  printd("Scheduler::Scheduler()");
}

void Scheduler::addProcess(Process *process)
{
  printd("Scheduler::addProcess; PID: " + to_string(process->getPID()));

  auto priority = process->getProcessPriority();

  if (priority == 0) {
    if (realtimeQueue.size() >= MAX_QUEUE_SIZE) {
      printd("Scheduler::addProcess - realtimeQueue is full");
      // TODO: throw exception
      return;
    }

    realtimeQueue.push(process);
    return;
  }

  if (userQueue1.size() < MAX_QUEUE_SIZE) {
    userQueue1.push(process);
    return;
  }

  if (userQueue2.size() < MAX_QUEUE_SIZE) {
    userQueue2.push(process);
    return;
  }

  if (userQueue3.size() < MAX_QUEUE_SIZE) {
    userQueue3.push(process);
    return;
  }

  printd("Scheduler::addProcess - all user queues are full");
}

void Scheduler::addProcess(vector<Process *> processes)
{
  printd("Scheduler::addProcess; size: " + to_string(processes.size()));

  for (auto process : processes) addProcess(process);
}

Process *Scheduler::removeProcess()
{
  printd("Scheduler::removeProcess");

  Process* process = nullptr;

  if (!realtimeQueue.empty()) {
    process = realtimeQueue.front();
    realtimeQueue.pop();
    return process;
  }

  if (!userQueue1.empty()) {
    process = userQueue1.top();
    userQueue1.pop();

    if (!userQueue2.empty()) {
      auto p = userQueue2.top();
      userQueue2.pop();
      userQueue1.push(p);
      
      if (!userQueue3.empty()) {
        p = userQueue3.top();
        userQueue3.pop();
        userQueue2.push(p);
      }
    }
  }

  if (process == nullptr) {
    printd("Scheduler::removeProcess - all queues are empty");
  }

  return process;
}
