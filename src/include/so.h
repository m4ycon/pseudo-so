#pragma once
#include "./common.h"
#include "./file/fileManager.h"
#include "./memory/memoryManager.h"
#include "./process/cpu.h"
#include "./resource/resourceManager.h"
#include "./scheduler/scheduler.h"
#include "./utils/fileReader.h"

class SO
{
public:
  SO();

  void exec();

private:
  CPU *cpu;

  FileManager *fileManager;
  MemoryManager *memoryManager;
  ResourceManager *resourceManager;
  Scheduler *scheduler;

  TimePoint startTime;
  vector<Process *> processesToArrive;

  std::mutex gettingProcessResourcesMutex;
  std::mutex freeingProcessResourcesMutex;

  void handleRealtimeProcess(Process *process);
  void handleUserProcess(Process *process);

  void deliverProcess(Process *process);
  void dispatcherPrint(Process *process);
  bool getProcessResources(Process *process);
  void freeProcessResources(Process *process);
};
