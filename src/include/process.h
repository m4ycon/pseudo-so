#pragma once
#include "./common.h"

using namespace std;

class Process
{
public:
  Process(int startup_time, int priority, int processor_time, int memory_blocks, bool printer_code_requested, bool scanner_request, bool modem_request, int disk_code);

  void run();

  void setPID(int PID) { this->PID = PID; }
  int getPID() { return this->PID; }

  int getProcessPriority() { return this->priority; }

  bool operator<(const Process& p) const {
    return this->priority < p.priority;
  }

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
};
