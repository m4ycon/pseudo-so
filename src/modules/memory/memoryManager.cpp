#include "../../include/memory/memoryManager.h"

/**
 * @brief Construct a new Memory Manager:: Memory Manager object
 * 
 * @param realtimeMemorySize 
 * @param userMemorySize 
 */
MemoryManager::MemoryManager(int realtimeMemorySize, int userMemorySize)
{
  this->realtimeMemorySize = realtimeMemorySize;
  this->userMemorySize = userMemorySize;

  for (int i = 0; i < this->realtimeMemorySize; i++)
    this->realtimeMemory[i] = -1;
  for (int i = 0; i < this->userMemorySize; i++)
    this->userMemory[i] = -1;
}

/**
 * @brief É passado um processo como argumento. Caso a memória esteja disponével,
 *        o processo é desalocado da memória e retorna true; caso o contrário, 
 *        ele retorna false.
 * @param process 
 * @return true 
 * @return false 
 */
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

/**
 * @brief É passado um processo como argumento o qual será alocado na memória.
 *        Caso seja bem sucedido, retorna true. Caso a memória não esteja 
 *        disponível ou não tenha espaço o suficiente, retorna false.
 * @param process 
 * @return true 
 * @return false 
 */
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


/**
 * @brief Verifica se o processo não é maior que o tamanho total da memória. 
 *        Caso seja, retorna false; caso o contrário, retorna true.
 *        
 * 
 * @param process 
 * @return true 
 * @return false 
 */
bool MemoryManager::isThereEnoughTotalMemory(Process *process)
{
  // no need to lock, because only checks total size and not what is inside
  const auto type = process->isRealtime() ? REALTIME : USER;
  const auto memoryBlockSize = process->getMemoryBlock();

  return type == REALTIME
    ? memoryBlockSize <= realtimeMemorySize
    : memoryBlockSize <= userMemorySize;
}

/**
 * @brief printa na tela o estado da memória naquele momento
 */
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

/**
 * @brief Dado o tipo de memória, procura um segmento livre de tamanho igual
 *        ou maior que o tamanho passado como argumento e retorna o indice do
 *        começo do segmento. Caso não encontre, retorna -1
 * 
 * @param size 
 * @param type 
 * @return int 
 */
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

/**
 * @brief Dado o tipo de memória, compacta a respectiva memória deixando
 *        os blocos vazios a direita (se existirem)
 * 
 * @param type 
 */
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
