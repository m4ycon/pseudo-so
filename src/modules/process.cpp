#include "../include/process.h"
#include "../include/utils.h"

Process::Process(int PID, int processPriority, int memoryOffset, int memorySize)
{
  print("Process::Process()");
  this->PID = PID;
  this->processPriority = processPriority;
  this->memoryOffset = memoryOffset;
  this->memorySize = memorySize;
}

void Process::run()
{
  print("Process::run(); PID: " + to_string(this->PID));
  auto time = Utils::randomDouble(0, 5);
  Utils::sleep(time);
}
