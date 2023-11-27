#pragma once
#include "./common.h"
#include "./file/fileManager.h"
#include "./memory/memoryManager.h"
#include "./resource/resourceManager.h"
#include "./scheduler/scheduler.h"
#include "./utils/fileReader.h"

class SO
{
public:
  SO();

  void exec();

private:
  FileManager *fileManager;
  MemoryManager *memoryManager;
  ResourceManager *resourceManager;
  Scheduler *scheduler;
};
