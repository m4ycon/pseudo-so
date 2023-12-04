#include "../../include/process/process.h"
#include "process.h"

int PID_COUNTER = 0;

/**
 * @brief Construct a new Process:: Process object
 * 
 * @param startup_time 
 * @param priority 
 * @param processor_time 
 * @param memory_blocks 
 * @param printer_code_requested 
 * @param scanner_request 
 * @param modem_request 
 * @param disk_code 
 */
Process::Process(int startup_time, int priority, int processor_time, int memory_blocks, bool printer_code_requested, bool scanner_request, bool modem_request, int disk_code)
{
  this->PID = PID_COUNTER++;
  this->priority = priority;
  this->startup_time = startup_time;
  this->processor_time = processor_time;
  this->memory_blocks = memory_blocks;
  this->printer_code_request = printer_code_requested;
  this->scanner_request = scanner_request;
  this->modem_request = modem_request;
  this->disk_code = disk_code;

  this->pc = 0;
}

/**
 * @brief Aumenta o valor da prioridade do processo. Caso sua prioriadade 
 *        seja 0 ou maior ou igual a 3, mantem o valor.
 */
void Process::increasePriority()
{
  if (this->priority == 0) return;
  if (this->priority >= 3) return;
  this->priority++;
}

/**
 * @brief adiciona uma nova instrução nas instruções do processo
 * 
 * @param instruction 
 */
void Process::addInstruction(Instruction *instruction)
{
  this->instructions.push_back(instruction);
}

/**
 * @brief Retorna a proxima instrução para ser executada do processo.
 *        Caso ele já esteja finalizado, retorna null
 * 
 * @return Instruction* 
 */
Instruction *Process::getActualInstruction()
{
  if (this->isFinished()) return NULL;

  return this->instructions[this->pc++];
}

/**
 * @brief Verifica se o processo terminou de executar. Se sim, retorna true;
 *        se não, retorna false.
 * 
 * @return true 
 * @return false 
 */
bool Process::isFinished()
{
  return this->pc >= this->instructions.size();
}
