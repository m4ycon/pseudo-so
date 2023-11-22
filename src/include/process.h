#pragma once
#include "./common.h"

using namespace std;

class Process
{
public:
  Process(int PID, int processPriority, int memoryOffset, int memorySize);

  void run();

  int getPID() { return this->PID; }
  int getProcessPriority() { return this->processPriority; }

  bool operator<(const Process& p) const {
    return this->processPriority < p.processPriority;
  }

private:
  int PID;
  int processPriority;
  int memoryOffset;
  int memorySize;

  bool usingPrinter;
  bool usingScanner;
  bool usingDrivers;
};
