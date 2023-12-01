#pragma once
#include "../common.h"
#include "../process/process.h"


enum MemoryType
{
  REALTIME,
  USER
};

class MemoryManager
{
public:
  MemoryManager(int realtimeMemorySize, int userMemorySize);

  bool freeMemory(Process *process);
  bool allocateMemory(Process *process);

  void printMemory();

private:
  int realtimeMemorySize, userMemorySize;
  int usedRealtimeMemorySize = 0, usedUserMemorySize = 0;

  int realtimeMemory[DEF_MEMORY_SIZE], userMemory[DEF_MEMORY_SIZE];

  std::mutex memoryMutex;

  int getContiguousIndexMemory(int size, MemoryType type);
  void compactMemory(MemoryType type);
};
