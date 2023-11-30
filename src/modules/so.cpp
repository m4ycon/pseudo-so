#include "../include/so.h"

SO::SO()
{
  this->startTime = Utils::now();
  auto fileReader = new FileReader();

  this->scheduler = new Scheduler(this->startTime);
  this->memoryManager = new MemoryManager(64, 1024 - 64);
  this->resourceManager = new ResourceManager();
  
  auto [fileManager, processesToArrive] = fileReader->setup("./input/processes.txt", "./input/files.txt", scheduler);
  this->fileManager = fileManager;
  this->processesToArrive = processesToArrive;

  this->cpu = new CPU(this->fileManager, this->startTime);

  for (auto process : this->processesToArrive) {
    thread t(&SO::deliverProcess, this, process);
    t.detach();
  }
}

void SO::exec()
{
  auto processes_finished = 0;

  while (processes_finished < (int) this->processesToArrive.size()) {
    auto process = this->scheduler->getNextProcess();
    if (process == nullptr) continue;

    auto elapsedTime = Utils::getElapsedTime(this->startTime);
    print("SO::exec - PID: " + to_string(process->getPID()) + "; Priority: " + to_string(process->getPriority()) + "; Time: " + to_string(elapsedTime) + "ms");

    if (process->getPriority() == 0) {
      this->handleRealtimeProcess(process);

      processes_finished++;
      print("SO::exec - finished; PID: " + to_string(process->getPID()) + "; Time: " + to_string(Utils::getElapsedTime(this->startTime)) + "ms");
      this->memoryManager->freeMemory(process);
    } else {
      this->handleUserProcess(process);

      if (!process->isFinished()) {
        this->scheduler->addReadyProcess(process);
      } else {
        processes_finished++;
        print("SO::exec - finished; PID: " + to_string(process->getPID()) + "; Time: " + to_string(Utils::getElapsedTime(this->startTime)) + "ms");
        this->memoryManager->freeMemory(process);
      }
    }
  }
}

void SO::handleRealtimeProcess(Process *process)
{
  while (!process->isFinished()) {
    this->cpu->execProcess(process);
  }
}

void SO::handleUserProcess(Process *process)
{
  auto startTime = Utils::now();
  while (!process->isFinished() && Utils::getElapsedTime(startTime) < QUANTUM) {
    this->cpu->execProcess(process);
  }
}

void SO::deliverProcess(Process *process)
{
  Utils::sleep(process->getStartupTime());
  bool allocateMemSuccess = this->memoryManager->alocateMemory(process);
  if (!allocateMemSuccess) {
    print("Não foi possível alocar memória para o processo " + to_string(process->getPID()));
    return ;
  }
  this->scheduler->addReadyProcess(process);
  print("SO::deliverProcess - PID: " + to_string(process->getPID()) + "; Priority: " + to_string(process->getPriority()) + "; Time: " + to_string(Utils::getElapsedTime(this->startTime)) + "ms");
}
