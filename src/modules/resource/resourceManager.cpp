#include "../../include/resource/resourceManager.h"


ResourceManager::ResourceManager()
{
  printd("ResourceManager::ResourceManager()");
}

bool ResourceManager::requestResource(Process *process)
{
  std::lock_guard<std::mutex> lock(resource_mutex);

  if (process->getPriority() == 0) return true;
  if (!checkEnoughResources(process)) return false;

  auto pid = process->getPID();

  auto is_printer_code_requested = process->getPrinterCodeRequest();
  auto is_scanner_requested = process->getScannerRequest();
  auto is_modem_requested = process->getModemRequest();
  auto disk_code = process->getDiskCode();

  if (is_printer_code_requested) {
    if (printer_code_request_pid[0] == -1) {
      printer_code_request_pid[0] = pid;
    } else if (printer_code_request_pid[1] == -1) {
      printer_code_request_pid[1] = pid;
    }
  }

  if (is_scanner_requested && scanner_request_pid == -1)
    scanner_request_pid = pid;

  if (is_modem_requested && modem_request_pid == -1)
    modem_request_pid = pid;

  if ((disk_code == 0 || disk_code == 1) && disk_request_pid[disk_code] == -1)
    disk_request_pid[disk_code] = pid;

  return true;
}

bool ResourceManager::releaseResource(Process *process)
{
  std::lock_guard<std::mutex> lock(resource_mutex);
  
  if (process->getPriority() == 0) return true;

  auto pid = process->getPID();

  auto is_printer_code_requested = process->getPrinterCodeRequest();
  auto is_scanner_requested = process->getScannerRequest();
  auto is_modem_requested = process->getModemRequest();
  auto disk_code = process->getDiskCode();

  if (is_printer_code_requested) {
    if (printer_code_request_pid[0] == pid) {
      printer_code_request_pid[0] = -1;
    } else if (printer_code_request_pid[1] == pid) {
      printer_code_request_pid[1] = -1;
    }
  }

  if (is_scanner_requested && scanner_request_pid == pid)
    scanner_request_pid = -1;

  if (is_modem_requested && modem_request_pid == pid)
    modem_request_pid = -1;

  if ((disk_code == 0 || disk_code == 1) && disk_request_pid[disk_code] == pid)
    disk_request_pid[disk_code] = -1;

  return true;
}

bool ResourceManager::checkEnoughResources(Process *process)
{
  if (process->getPriority() == 0) return true;

  auto is_printer_code_requested = process->getPrinterCodeRequest();
  auto is_scanner_requested = process->getScannerRequest();
  auto is_modem_requested = process->getModemRequest();
  auto disk_code = process->getDiskCode();

  if (is_printer_code_requested 
    && printer_code_request_pid[0] != -1
    && printer_code_request_pid[1] != -1
  )
    return false;

  if (is_scanner_requested && scanner_request_pid != -1)
    return false;

  if (is_modem_requested && modem_request_pid != -1)
    return false;

  if ((disk_code == 0 || disk_code == 1) && disk_request_pid[disk_code] != -1)
    return false;

  return true;
}
