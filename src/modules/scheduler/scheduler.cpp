#include "../../include/scheduler/scheduler.h"


Scheduler::Scheduler(TimePoint startTime)
{
  printd("Scheduler::Scheduler()");

  this->startTime = startTime;
}

void Scheduler::addReadyProcess(Process *process)
{
  printd("Scheduler::addReadyProcess; PID: " + to_string(process->getPID()));
  this->readyQueue.push(process);
}

bool Scheduler::isEmpty()
{
  auto totalSize = readyQueue.size() + realtimeQueue.size() + userQueue1.size() + userQueue2.size() + userQueue3.size();
  return totalSize == 0;
}

Process *Scheduler::getNextProcess()
{
  printd("Scheduler::getNextProcess");

  // check if there is any process in the ready queue
  if (!readyQueue.empty()) {
    auto process = readyQueue.front();
    readyQueue.pop();
    this->addProcess(process);
  }

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
    printd("Scheduler::getNextProcess - all queues are empty");
  }

  return process;
}

void Scheduler::addProcess(Process *process)
{
  printd("Scheduler::addProcess; PID: " + to_string(process->getPID()));

  auto priority = process->getPriority();

  if (priority == 0) {
    if (realtimeQueue.size() >= MAX_QUEUE_SIZE) {
      printd("Scheduler::addProcess - realtimeQueue is full");
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
