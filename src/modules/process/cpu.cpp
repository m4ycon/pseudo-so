#include "../../include/process/cpu.h"

/**
 * @brief Construct a new CPU::CPU object
 * 
 * @param fileManager 
 * @param startTime 
 */
CPU::CPU(FileManager *fileManager, TimePoint startTime)
{
  this->fileManager = fileManager;
  this->startTime = startTime;
}

/**
 * @brief Põe um processo para executar a sua próxima instrução
 * 
 * @param process 
 */
void CPU::execProcess(Process *process)
{
  this->execInstruction(process->getActualInstruction());
}

/**
 * @brief Executa uma instrução que pode ser de criar ou deletar um arquivo.
 *        Caso não seja nenhum desses, printa na tela que foi executado uma
 *        instrução inválida.
 * 
 * @param instruction 
 */
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

/**
 * @brief Executa a instrução de criar um arquivo. Caso seja bem sucedido printa
 *        na tela um feedback da criação. Caso contrário, printa o motivo de não
 *        ter criado o arquivo: falta de espaço ou arquivo ja existente.
 * 
 * @param instruction 
 */
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

  if (resCode == FILE_ALREADY_EXISTS) {
    print("O processo " + to_string(instruction->pid) + " não pode criar o arquivo " + instruction->filename + " (arquivo já existe).");
    return;
  }
}

/**
 * @brief Executa a instrução de deletar um arquivo. Caso seja bem sucedido 
 *        printa na tela um feedback da deleção. Caso contrário, printa 
 *        o motivo de não ter deletado o arquivo: arquivo não encontrado.
 * 
 * @param instruction 
 */
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
