#include "../../include/memory/memoryManager.h"


std::mutex memoryMutex;

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

void MemoryManager::freeMemory(Process *process)
{
  std::lock_guard<std::mutex> lock(memoryMutex);
  if (process->getPriority() == 0) {
    usedRealtimeMemorySize -= process->getMemoryBlock();

    for (int i = 0; i < process->getMemoryBlock(); i++)
      this->realtimeMemory[process->getOffset() + i] = -1;

  } else {
    usedUserMemorySize -= process->getMemoryBlock();

    for (int i = 0; i < process->getMemoryBlock(); i++)
      this->userMemory[process->getOffset() + i - 64] = -1;

  }

}

bool MemoryManager::allocateMemory(Process *process)
{
  std::lock_guard<std::mutex> lock(memoryMutex);
  if (process->getPriority() == 0) {
    if (usedRealtimeMemorySize + process->getMemoryBlock() > realtimeMemorySize) {
      return false;
    } else {
      usedRealtimeMemorySize += process->getMemoryBlock();

      auto startPos = getContiguousIndexMemory(process->getMemoryBlock(), "real");
      if (startPos == -1) {
        compactMemoryReal();
        startPos = getContiguousIndexMemory(process->getMemoryBlock(), "real");
      }

      for (int i = 0; i < process->getMemoryBlock(); i++)
        this->realtimeMemory[startPos + i] = 1;
      process->setOffset(startPos);

      return true;
    }
  } else {
    if (usedUserMemorySize + process->getMemoryBlock() > userMemorySize) {
      return false;
    } else {
      usedUserMemorySize += process->getMemoryBlock();

      auto startPos = getContiguousIndexMemory(process->getMemoryBlock(), "user");
      if (startPos == -1) {
        compactMemoryUser();
        startPos = getContiguousIndexMemory(process->getMemoryBlock(), "user");
      }

      for (int i = 0; i < process->getMemoryBlock(); i++)
        this->userMemory[startPos + i] = 1;
      process->setOffset(startPos);

      process->setOffset( startPos + 64);
      return true;
    }
  }
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

int MemoryManager::getContiguousIndexMemory(int size, string type) // TODO: fazer com ponteiros mais simplificado
{ 
  int emptyCount = 0, startPos = -1;

  if (type == "real") {
    for (int i = 0; i < this->realtimeMemorySize; ++i) {
      if (this->realtimeMemory[i] == -1) {
        if (startPos == -1) {
          startPos = i;
        }
        emptyCount++;
      } else {
        if (emptyCount >= size) break;
        emptyCount = 0, startPos = -1;
      }
    }
  } else {
    for (int i = 0; i < this->userMemorySize; ++i) {
      if (this->userMemory[i] == -1) {
        if (startPos == -1) {
          startPos = i;
        }
        emptyCount++;
      } else {
        if (emptyCount >= size) break;
        emptyCount = 0, startPos = -1;
      }
    }
  }

  return startPos;
}

void MemoryManager::compactMemoryReal() // TODO: fazer com ponteiros mais simplificado
{
  int writeIndex = 0;

  for (int i = 0; i < this->realtimeMemorySize; i++) {
    if (this->realtimeMemory[i] != -1)
      this->realtimeMemory[writeIndex++] = this->realtimeMemory[i];
  }

  for (; writeIndex < this->realtimeMemorySize; writeIndex++) {
    this->realtimeMemory[writeIndex] = -1;
  }
}

void MemoryManager::compactMemoryUser() 
{
  int writeIndex = 0;

  for (int i = 0; i < this->userMemorySize; i++) {
    if (this->userMemory[i] != -1)
      this->userMemory[writeIndex++] = this->userMemory[i];
  }

  for (; writeIndex < this->userMemorySize; writeIndex++) {
    this->userMemory[writeIndex] = -1;
  }
}
