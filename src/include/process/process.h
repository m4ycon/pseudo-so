#pragma once
#include "../common.h"
#include "./instruction.h"

class Process
{
public:
  Process(int startup_time, int priority, int processor_time, int memory_blocks, bool printer_code_requested, bool scanner_request, bool modem_request, int disk_code);

  void run();

  int getPID() { return this->PID; }
  int getPriority() { return this->priority; }
  int getStartupTime() { return this->startup_time; }

  void increasePriority();

  void addInstruction(Instruction *instruction);
  Instruction *getActualInstruction();
  bool isFinished();

  bool operator<(const Process &p) const { return this->priority < p.priority; }

private:
  int PID;
  int priority;

  int startup_time;
  int processor_time;
  int memory_blocks;

  bool printer_code_requested;
  bool scanner_request;
  bool modem_request;

  int disk_code;

  vector<Instruction *> instructions;
  uint16_t pc = 0;
};
