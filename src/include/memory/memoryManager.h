#pragma once
#include "../common.h"
#include "../process/process.h"

/* Memória de 1024 blocos. Dessa quantidade, 64 blocos devem
ser reservados para processos de tempo-real e os 960 blocos restantes devem ser compartilhados entre os
processos de usuário */
struct MemoryBlock
{
  int realMemory = 0;
  int userMemory = 0;

};

class MemoryManager
{
public:
  MemoryManager(int realtimeMemorySize, int userMemorySize);

  void freeMemory(Process *process);
  bool alocateMemory(Process *process);

  void printMemory();

private:
  int realtimeMemorySize, userMemorySize;
  int usedRealtimeMemorySize = 0, usedUserMemorySize = 0;

  char realtimeMemory[DEF_MEMORY_SIZE], userMemory[DEF_MEMORY_SIZE];
};
