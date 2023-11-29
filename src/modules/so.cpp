#include "../include/so.h"

SO::SO()
{
  this->startTime = chrono::steady_clock::now();
  auto fileReader = new FileReader();

  this->scheduler = new Scheduler(this->startTime);
  this->memoryManager = new MemoryManager();
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
    
    if (process != nullptr) {
      this->cpu->execProcess(process);
      
      if (!process->isFinished()) {
        process->increasePriority();
        this->scheduler->addReadyProcess(process);
        continue;
      }

      // TODO: this->memoryManager->freeMemory(process);
      processes_finished++;
    }
  }
}
void SO::deliverProcess(Process *process)
{
  Utils::sleep(process->getStartupTime());
  // TODO: this->memoryManager->alocateMemory(process);
  this->scheduler->addReadyProcess(process);
}
