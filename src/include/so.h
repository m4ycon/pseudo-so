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
  SO(string processes_path, string files_path);

  void exec();

private:
  CPU *cpu;

  FileManager *fileManager;
  MemoryManager *memoryManager;
  ResourceManager *resourceManager;
  Scheduler *scheduler;

  TimePoint startTime;
  int numProcessesToArrive;

  std::mutex gettingProcessResourcesMutex;

  void handleRealtimeProcess(Process *process);
  void handleUserProcess(Process *process);

  void deliverProcess(Process *process);
  bool getProcessResources(Process *process);
  void freeProcessResources(Process *process);
  bool isThereEnoughResources(Process *process);
  void dispatcherPrint(Process *process);
};
