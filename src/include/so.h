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

  void deliverProcess(Process *process);
};
