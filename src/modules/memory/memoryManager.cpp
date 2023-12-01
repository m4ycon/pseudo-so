#include "../../include/memory/memoryManager.h"


MemoryManager::MemoryManager(int realtimeMemorySize, int userMemorySize)
{
  printd("MemoryManager::MemoryManager()");
  this->realtimeMemorySize = realtimeMemorySize;
  this->userMemorySize = userMemorySize;

  for (int i = 0; i < this->realtimeMemorySize; i++)
    this->realtimeMemory[i] = -1;
  for (int i = 0; i < this->userMemorySize; i++)
    this->userMemory[i] = -1;
}

bool MemoryManager::freeMemory(Process *process)
{
  if (!memoryMutex.try_lock()) return false;
  std::lock_guard<std::mutex> lock(memoryMutex, std::adopt_lock);

  if (process->getPriority() == 0) {
    usedRealtimeMemorySize -= process->getMemoryBlock();

    for (int i = 0; i < process->getMemoryBlock(); i++)
      this->realtimeMemory[process->getOffset() + i] = -1;

    this->processOffsets.erase(process->getPID());
  } else {
    usedUserMemorySize -= process->getMemoryBlock();

    for (int i = 0; i < process->getMemoryBlock(); i++)
      this->userMemory[process->getOffset() + i - this->realtimeMemorySize] = -1;

    this->processOffsets.erase(process->getPID());
  }

  return true;
}

bool MemoryManager::allocateMemory(Process *process)
{
  if (!memoryMutex.try_lock()) return false;
  std::lock_guard<std::mutex> lock(memoryMutex, std::adopt_lock);

  auto pid = process->getPID();

  if (process->getPriority() == 0) {
    if (usedRealtimeMemorySize + process->getMemoryBlock() > realtimeMemorySize) {
      return false;
    } else {
      usedRealtimeMemorySize += process->getMemoryBlock();

      auto startPos = getContiguousIndexMemory(process->getMemoryBlock(), REALTIME);
      if (startPos == -1) {
        this->compactMemory(REALTIME);
        startPos = getContiguousIndexMemory(process->getMemoryBlock(), REALTIME);
      }

      for (int i = 0; i < process->getMemoryBlock(); i++)
        this->realtimeMemory[startPos + i] = pid;

      processOffsets[pid] = new int(startPos);
      process->setOffset(processOffsets[pid]);
      return true;
    }
  } else {
    if (usedUserMemorySize + process->getMemoryBlock() > userMemorySize) {
      return false;
    } else {
      usedUserMemorySize += process->getMemoryBlock();

      auto startPos = getContiguousIndexMemory(process->getMemoryBlock(), USER);
      if (startPos == -1) {
        this->compactMemory(USER);
        startPos = getContiguousIndexMemory(process->getMemoryBlock(), USER);
      }

      for (int i = 0; i < process->getMemoryBlock(); i++)
        this->userMemory[startPos + i] = pid;

      processOffsets[pid] = new int(startPos + this->realtimeMemorySize);
      process->setOffset(processOffsets[pid]);
      return true;
    }
  }
}

void MemoryManager::printMemory()
{
  printd("FileManager::printDisk()");

  string printStr = "";
  for (int i = 0; i < this->realtimeMemorySize; i++)
    printStr += to_string(this->realtimeMemory[i]) + SEPARATOR;
  for (int i = 0; i < this->userMemorySize; i++)
    printStr += to_string(this->userMemory[i]) + SEPARATOR;
  print(printStr);

  // print map offset
  printStr = "";
  for (auto it = this->processOffsets.begin(); it != this->processOffsets.end(); ++it) {
    printStr += to_string(it->first) + ":" + to_string(*it->second) + SEPARATOR;
  }
  print(printStr);
}

int MemoryManager::getContiguousIndexMemory(int size, MemoryType type) // TODO: fazer com ponteiros mais simplificado
{ 
  int emptyCount = 0, startPos = -1;
  auto memory = type == REALTIME ? this->realtimeMemory : this->userMemory;
  auto memorySize = type == REALTIME ? this->realtimeMemorySize : this->userMemorySize;

  for (int i = 0; i < memorySize; ++i) {
    if (memory[i] == -1) {
      if (startPos == -1) {
        startPos = i;
      }
      emptyCount++;
    } else {
      if (emptyCount >= size) break;
      emptyCount = 0, startPos = -1;
    }
  }

  if (emptyCount >= size) return startPos;
  return -1;
}

void MemoryManager::compactMemory(MemoryType type)
{
  int writeIndex = 0;
  auto memory = type == REALTIME ? this->realtimeMemory : this->userMemory;
  auto memorySize = type == REALTIME ? this->realtimeMemorySize : this->userMemorySize;

  auto lastPID = -1;
  for (int i = 0; i < memorySize; i++) {
    if (memory[i] != -1) {
      memory[writeIndex++] = memory[i];
      
      // update offset of process
      if (lastPID != memory[i]) {
        lastPID = memory[i];
        *processOffsets[lastPID] = writeIndex - 1;
      }
    }
  }

  for (; writeIndex < memorySize; writeIndex++) {
    memory[writeIndex] = -1;
  }
}
