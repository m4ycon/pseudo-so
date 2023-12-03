#pragma once
#include "../common.h"
#include "./process.h"
#include "../file/fileManager.h"
#include "../file/file.h"

class CPU
{
public:
  CPU(FileManager *fileManager, TimePoint startTime);

  void execProcess(Process *process);

private:
  TimePoint startTime;
  FileManager *fileManager;

  void execInstruction(Instruction *instruction);
  void opcode0(Instruction *instruction);
  void opcode1(Instruction *instruction);
};
