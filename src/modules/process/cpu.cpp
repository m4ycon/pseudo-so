#include "../../include/process/cpu.h"


CPU::CPU(FileManager *fileManager, TimePoint startTime)
{
  this->fileManager = fileManager;
  this->startTime = startTime;
}

void CPU::execProcess(Process *process)
{
  const auto elapsedTime = Utils::getElapsedTime(this->startTime);
  print("CPU::execProcess; PID: " + to_string(process->getPID()) + "; Time: " + to_string(elapsedTime) + "ms");

  this->execInstruction(process->getActualInstruction());
}

void CPU::execInstruction(Instruction *instruction)
{
  switch (instruction->opcode)
  {
  case 0:
    opcode0(instruction);
    break;
  case 1:
    opcode1(instruction);
    break;
  default:
    print("O processo " + to_string(instruction->pid) + " executou uma instrução inválida.");
    break;
  }
}

void CPU::opcode0(Instruction *instruction)
{
  // create file
  auto resCode = this->fileManager->addFile(new File(instruction->filename, 0, instruction->numBlocks));
  
  if (resCode == OK) {
    auto file = this->fileManager->getFile(instruction->filename);
    
    auto printStr = "O processo " + to_string(instruction->pid) + " criou o arquivo " + file->filename + " (blocos ";
    for (int i = 0; i < file->size; i++)
      printStr += to_string(file->start_block + i) + (i + 1 != file->size ? ", " : ").");

    print(printStr);
    return;
  }
  
  if (resCode == NOT_ENOUGH_SPACE) {
    print("O processo " + to_string(instruction->pid) + " não pode criar o arquivo " + instruction->filename + " (falta de espaço).");
    return;
  }
}

void CPU::opcode1(Instruction *instruction)
{
  // delete file
  auto resCode = this->fileManager->removeFile(instruction->filename);

  if (resCode == OK) {
    print("O processo " + to_string(instruction->pid) + " deletou o arquivo " + instruction->filename + ".");
    return;
  }
  
  if (resCode == FILE_NOT_FOUND) {
    print("O processo " + to_string(instruction->pid) + " não pode deletar o arquivo " + instruction->filename + " porque ele não existe.");
    return;
  }
}
