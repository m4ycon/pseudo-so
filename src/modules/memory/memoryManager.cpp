#include "../../include/memory/memoryManager.h"


MemoryManager::MemoryManager(int realtimeMemorySize, int userMemorySize)
{
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

  auto pid = process->getPID();
  auto is_realtime = process->isRealtime();
  auto memory_block = process->getMemoryBlock();
  auto used_memory_size = is_realtime ? &usedRealtimeMemorySize : &usedUserMemorySize;
  auto memory = is_realtime ? realtimeMemory : userMemory;

  *used_memory_size -= memory_block;

  for (int i = 0; i < memory_block; i++)
    memory[process->getOffset() + i - (is_realtime ? 0 : this->realtimeMemorySize)] = -1;

  this->processOffsets.erase(pid);
  return true;
}

bool MemoryManager::allocateMemory(Process *process)
{
  if (!memoryMutex.try_lock()) return false;
  std::lock_guard<std::mutex> lock(memoryMutex, std::adopt_lock);

  auto pid = process->getPID();
  auto is_realtime = process->isRealtime();
  auto memory_block = process->getMemoryBlock();
  auto memory_type = is_realtime ? REALTIME : USER;
  auto memory_size = is_realtime ? &realtimeMemorySize : &userMemorySize;
  auto used_memory_size = is_realtime ? &usedRealtimeMemorySize : &usedUserMemorySize;
  auto memory = is_realtime ? realtimeMemory : userMemory;

  if (*used_memory_size + memory_block > *memory_size)
    return false;
  *used_memory_size += memory_block;

  auto startPos = getContiguousIndexMemory(memory_block, memory_type);
  if (startPos == -1) {
    this->compactMemory(memory_type);
    startPos = getContiguousIndexMemory(memory_block, memory_type);
  }

  for (int i = 0; i < memory_block; i++)
    memory[startPos + i] = pid;

  processOffsets[pid] = new int(startPos + (is_realtime ? 0 : this->realtimeMemorySize));
  process->setOffset(processOffsets[pid]);
  return true;
}

bool MemoryManager::isThereEnoughTotalMemory(Process *process)
{
  // no need to lock, because only checks total size and not what is inside
  const auto type = process->isRealtime() ? REALTIME : USER;
  const auto memoryBlockSize = process->getMemoryBlock();

  return type == REALTIME
    ? memoryBlockSize <= realtimeMemorySize
    : memoryBlockSize <= userMemorySize;
}

void MemoryManager::printMemory()
{
  string separator(1, SEPARATOR);
  string printStr = separator;
  for (int i = 0; i < this->realtimeMemorySize; i++)
    printStr += to_string(this->realtimeMemory[i]) + separator;
  for (int i = 0; i < this->userMemorySize; i++)
    printStr += to_string(this->userMemory[i]) + separator;
  print(printStr);
}

int MemoryManager::getContiguousIndexMemory(int size, MemoryType type)
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
