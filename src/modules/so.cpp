#include "../include/so.h"

SO::SO()
{
  auto fileReader = new FileReader();

  this->scheduler = new Scheduler();
  this->memoryManager = new MemoryManager();
  this->resourceManager = new ResourceManager();
  this->fileManager = fileReader->setup("./input/processes.txt", "./input/files.txt", scheduler);
}

void SO::exec()
{
  while (!this->scheduler->realtimeQueue.empty()) {
    auto process = this->scheduler->removeProcess();
    execInstruction(process->getActualInstruction());

    if (!process->isFinished()) this->scheduler->addProcess(process);
  }
}

void SO::execInstruction(Instruction *instruction)
{
  if (instruction->opcode == 0) {
    // create
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

  // delete
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
