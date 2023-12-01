#include "../../include/process/process.h"
#include "process.h"

int PID_COUNTER = 0;

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

void Process::run()
{
  print("Process::run(); PID: " + to_string(this->PID) + "; start");
  auto sleepTime = Utils::randomDouble(1, 5);
  Utils::sleep(sleepTime);
  print("Process::run(); PID: " + to_string(this->PID) + "; execution time: " + to_string(sleepTime));
}

void Process::increasePriority()
{
  if (this->priority == 0) return;
  if (this->priority >= 3) return;
  this->priority++;
}

void Process::addInstruction(Instruction *instruction)
{
  this->instructions.push_back(instruction);
}

Instruction *Process::getActualInstruction()
{
  if (this->isFinished()) return NULL;

  return this->instructions[this->pc++];
}

bool Process::isFinished()
{
  return this->pc >= this->instructions.size();
}
