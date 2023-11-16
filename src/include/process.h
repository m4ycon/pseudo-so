#pragma once
#include "./common.h"

class Process
{
public:
  Process(int PID, int processPriority, int memoryOffset, int memorySize);

  void run();

private:
  int PID;
  int processPriority;
  int memoryOffset;
  int memorySize;

  bool usingPrinter;
  bool usingScanner;
  bool usingDrivers;
};
