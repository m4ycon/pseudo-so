#pragma once
#include "../common.h"
#include "../process/process.h"

class MemoryManager
{
public:
  MemoryManager(int realtimeMemorySize, int userMemorySize);

  void freeMemory(Process *process);
  void alocateMemory(Process *process);

  void printMemory();

private:
  int realtimeMemorySize, userMemorySize;
  char realtimeMemory[DEF_MEMORY_SIZE], userMemory[DEF_MEMORY_SIZE];
};
