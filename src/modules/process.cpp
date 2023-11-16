#include "../include/process.h"

Process::Process(int PID, int processPriority, int memoryOffset, int memorySize)
{
  printd("Process::Process(); PID: " + std::to_string(PID));
  this->PID = PID;
  this->processPriority = processPriority;
  this->memoryOffset = memoryOffset;
  this->memorySize = memorySize;
}

void Process::run()
{
  print("Process::run(); PID: " + std::to_string(this->PID) + "; start");
  auto sleepTime = Utils::randomDouble(1, 5);
  Utils::sleep(sleepTime);
  print("Process::run(); PID: " + std::to_string(this->PID) + "; execution time: " + std::to_string(sleepTime));
}
