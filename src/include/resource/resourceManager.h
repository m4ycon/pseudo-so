#pragma once
#include "../common.h"
#include "../process/process.h"

class ResourceManager
{
public:
  ResourceManager();

  bool requestResource(Process *process);
  bool releaseResource(Process *process);

private:
  int printer_code_request_pid[2] = {-1, -1};
  int scanner_request_pid = -1;
  int modem_request_pid = -1;
  int disk_request_pid[2] = {-1, -1};

  std::mutex resource_mutex;

  bool checkEnoughResources(Process *process);
};
