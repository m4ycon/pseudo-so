#include "../../include/scheduler/scheduler.h"


Scheduler::Scheduler(TimePoint startTime)
{

  this->startTime = startTime;
}

void Scheduler::addReadyProcess(Process *process, bool increasePriority)
{
  std::lock_guard<std::mutex> lock(schedulerMutex);
  if (increasePriority) process->increasePriority();
  this->readyQueue.push(process);
}

Process *Scheduler::getNextProcess()
{
  std::lock_guard<std::mutex> lock(schedulerMutex);

  // check if there is any process in the ready queue
  while (!readyQueue.empty()) {
    auto process = readyQueue.front();
    readyQueue.pop();
    this->addProcess(process);
  }

  if (!realtimeQueue.empty()) {
    auto process = realtimeQueue.front();
    realtimeQueue.pop();
    return process;
  }

  if (!userQueue1.empty()) {
    auto process = userQueue1.front();
    userQueue1.pop();
    return process;
  }

  if (!userQueue2.empty()) {
    auto process = userQueue2.front();
    userQueue2.pop();
    return process;
  }

  if (!userQueue3.empty()) {
    auto process = userQueue3.front();
    userQueue3.pop();
    return process;
  }

  return nullptr;
}

void Scheduler::addProcess(Process *process)
{
  if (process->isRealtime() && realtimeQueue.size() < MAX_QUEUE_SIZE) {
    realtimeQueue.push(process);
    return;
  }

  auto priority = process->getPriority();
  if (priority == 1 && userQueue1.size() < MAX_QUEUE_SIZE) {
    userQueue1.push(process);
    return;
  }

  if (priority == 2 && userQueue2.size() < MAX_QUEUE_SIZE) {
    userQueue2.push(process);
    return;
  }

  if (priority >= 3 && userQueue3.size() < MAX_QUEUE_SIZE) {
    userQueue3.push(process);
    return;
  }

  this->readyQueue.push(process);
}
