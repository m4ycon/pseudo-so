#include "../../include/memory/memoryManager.h"


std::mutex memoryMutex;

MemoryManager::MemoryManager(int realtimeMemorySize, int userMemorySize)
{
  printd("MemoryManager::MemoryManager()");
  this->realtimeMemorySize = realtimeMemorySize;
  this->userMemorySize = userMemorySize;

  for (int i = 0; i < this->realtimeMemorySize; i++)
    this->realtimeMemory[i] = EMPTY_BLOCK;
  for (int i = 0; i < this->userMemorySize; i++)
    this->userMemory[i] = EMPTY_BLOCK;
}

void MemoryManager::freeMemory(Process *process)
{
  std::lock_guard<std::mutex> lock(memoryMutex);
  // TODO: implement
}

void MemoryManager::alocateMemory(Process *process)
{
  std::lock_guard<std::mutex> lock(memoryMutex);
  // TODO: implement
}

void MemoryManager::printMemory()
{
  printd("FileManager::printDisk()");

  string printStr = to_string(SEPARATOR);
  for (int i = 0; i < this->realtimeMemorySize; i++)
    printStr += this->realtimeMemory[i] + SEPARATOR;
  for (int i = 0; i < this->userMemorySize; i++)
    printStr += this->userMemory[i] + SEPARATOR;
  print(printStr);
}
