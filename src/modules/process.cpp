#include "../include/process.h"

Process::Process(int startup_time, int priority, int processor_time, int memory_blocks, bool printer_code_requested, bool scanner_request, bool modem_request, int disk_code)
{
  this->PID = 0;
  this->priority = priority;
  this->startup_time = startup_time;
  this->processor_time = processor_time;
  this->memory_blocks = memory_blocks;
  this->printer_code_requested = printer_code_requested;
  this->scanner_request = scanner_request;
  this->modem_request = modem_request;
  this->disk_code = disk_code;
}

void Process::run()
{
  print("Process::run(); PID: " + to_string(this->PID) + "; start");
  auto sleepTime = Utils::randomDouble(1, 5);
  Utils::sleep(sleepTime);
  print("Process::run(); PID: " + to_string(this->PID) + "; execution time: " + to_string(sleepTime));
}
