#include "../../include/scheduler/scheduler.h"


std::mutex schedulerMutex;

Scheduler::Scheduler(TimePoint startTime)
{
  printd("Scheduler::Scheduler()");

  this->startTime = startTime;
}

void Scheduler::addReadyProcess(Process *process)
{
  std::lock_guard<std::mutex> lock(schedulerMutex);
  printd("Scheduler::addReadyProcess; PID: " + to_string(process->getPID()));
  this->readyQueue.push(process);
}

bool Scheduler::isEmpty()
{
  std::lock_guard<std::mutex> lock(schedulerMutex);
  auto totalSize = readyQueue.size() + realtimeQueue.size() + userQueue1.size() + userQueue2.size() + userQueue3.size();
  return totalSize == 0;
}

Process *Scheduler::getNextProcess()
{
  printd("Scheduler::getNextProcess");
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

  printd("Scheduler::getNextProcess - all queues are empty");
  return nullptr;
}

void Scheduler::addProcess(Process *process)
{
  printd("Scheduler::addProcess; PID: " + to_string(process->getPID()));

  auto priority = process->getPriority();

  if (priority == 0 && realtimeQueue.size() < MAX_QUEUE_SIZE) {
    realtimeQueue.push(process);
    return;
  }

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

  printd("Scheduler::addProcess - all queues are full, adding to ready queue");
}
