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

    if (process->isRealtime()) {
      this->handleRealtimeProcess(process);

      processes_finished++;
      print("SO::exec - finished; PID: " + to_string(process->getPID()) + "; Time: " + to_string(Utils::getElapsedTime(this->startTime)) + "ms");
      this->freeProcessResources(process);
    } else {
      this->handleUserProcess(process);

      if (!process->isFinished()) {
        this->scheduler->addReadyProcess(process);
      } else {
        processes_finished++;
        print("SO::exec - finished; PID: " + to_string(process->getPID()) + "; Time: " + to_string(Utils::getElapsedTime(this->startTime)) + "ms");
        this->freeProcessResources(process);
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
  if (!this->isThereEnoughResources(process)) {
    printd("Não há recursos suficientes para o processo. PID: " + to_string(process->getPID()));
    return;
  }

  bool gotResources = false;
  while (!gotResources) {
    gotResources = this->getProcessResources(process);
  }
  dispatcherPrint(process);

  this->scheduler->addReadyProcess(process);
  print("SO::deliverProcess - PID: " + to_string(process->getPID()) + "; Priority: " + to_string(process->getPriority()) + "; Time: " + to_string(Utils::getElapsedTime(this->startTime)) + "ms");
}

bool SO::getProcessResources(Process *process)
{
  std::lock_guard<std::mutex> lock(this->gettingProcessResourcesMutex);

  bool allocateMemSuccess = this->memoryManager->allocateMemory(process);
  if (!allocateMemSuccess) {
    printd("Não foi possível alocar memória para o processo. PID: " + to_string(process->getPID()));
    return false;
  }
  
  bool requestResourceSuccess = this->resourceManager->requestResource(process);
  if (!requestResourceSuccess) {
    while (!this->memoryManager->freeMemory(process));
    printd("Não foi possível alocar recursos para o processo. PID: " + to_string(process->getPID()));
    return false;
  }

  return true;
}

void SO::freeProcessResources(Process *process)
{
  std::lock_guard<std::mutex> lock(this->freeingProcessResourcesMutex);
  while (!this->memoryManager->freeMemory(process));
  while (!this->resourceManager->freeResource(process));
}

bool SO::isThereEnoughResources(Process *process)
{
  bool enough_memory = this->memoryManager->isThereEnoughMemory(process);
  // no need to check in resourceManager

  return enough_memory;
}

void SO::dispatcherPrint(Process *process)
{
  /*
  dispatcher =>
    PID: 0
    offset: 0
    blocks: 64
    priority: 0
    time: 3
    printers: 0
    scanners: 0
    modems: 0
    drives: 0

  process 0 =>
  P0 STARTED
  P0 instruction 1
  P0 instruction 2
  P0 instruction 3
  P0 return SIGINT
  */

  string PID = to_string(process->getPID());
  string priority = to_string(process->getPriority());  
  string memoryBlocks = to_string(process->getMemoryBlock());
  string processor_time = to_string(process->getProcessorTime());
  string printer_code_requested = process->getPrinterCodeRequest() ? "1" : "0";
  string scanner_request = process->getScannerRequest() ? "1" : "0";
  string modem_request = process->getModemRequest() ? "1" : "0";
  string instructions = "";
  string offset = to_string(process->getOffset());

  for (int i = 1; i <= process->getProcessorTime(); i++) {
    instructions += "P" + PID + " instruction " + to_string(i) + "\n";
  }

  print("\ndispatcher =>\n" 
    "    PID: " + PID + "\n"
    "    offset: " + offset + "\n"
    "    blocks: " + memoryBlocks + "\n"
    "    priority: " + priority + "\n"
    "    time: " + processor_time + "\n"
    "    printers: " + printer_code_requested + "\n"
    "    scanners: " + scanner_request + "\n"
    "    modems: " + modem_request + "\n"
    "    drives: 0\n\n"
    "process " + PID + " =>\n"
    "P" + PID + " STARTED" + "\n"
    "" + instructions + ""
    "P" + PID + " return SIGINT" + "\n"
  );
}

