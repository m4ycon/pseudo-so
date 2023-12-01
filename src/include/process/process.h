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
  int getMemoryBlock() { return this->memory_blocks; }
  int getProcessorTime() { return this->processor_time; }

  bool getPrinterCodeRequest() { return this->printer_code_request; }
  bool getScannerRequest() { return this->scanner_request; }
  bool getModemRequest() { return this->modem_request; }
  int getDiskCode() { return this->disk_code; }

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

  bool printer_code_request;
  bool scanner_request;
  bool modem_request;
  int disk_code;

  vector<Instruction *> instructions;
  uint16_t pc = 0;
};
