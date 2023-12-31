#include "../../include/resource/resourceManager.h"

/**
 * @brief Construct a new Resource Manager:: Resource Manager object
 */
ResourceManager::ResourceManager()
{
}

/**
 * @brief O processo solicita um recuso. Caso seja um processo realtime,
 *        retorna true. Caso o(s) recuso(s) requerido(s) pelo processo de 
 *        usuário estejam disponíveis, retorna true. Caso um outro processo 
 *        esteja requerendo no momento um recuso ou os recusos requeridos não 
 *        estão disponíveis, retorna false.
 * 
 * @param process 
 * @return true 
 * @return false 
 */
bool ResourceManager::requestResource(Process *process)
{
  if (!resource_mutex.try_lock()) return false;
  std::lock_guard<std::mutex> lock(resource_mutex, std::adopt_lock);

  if (process->isRealtime()) return true;
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

  if (disk_code > 0) {
    if (disk_request_pid[0] == -1) {
      disk_request_pid[0] = pid;
    } else if (disk_request_pid[1] == -1) {
      disk_request_pid[1] = pid;
    }
  }

  return true;
}

/**
 * @brief Um processo libera os recusos que estava utilizando. Caso um outro
 *        processo esteja requerendo/liberando recusos no mesmo momento, 
 *        retorna false. Caso o contrário, retorna true.
 * 
 * @param process 
 * @return true 
 * @return false 
 */
bool ResourceManager::freeResource(Process *process)
{
  if (!resource_mutex.try_lock()) return false;
  std::lock_guard<std::mutex> lock(resource_mutex, std::adopt_lock);
  
  if (process->isRealtime()) return true;

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

  if (disk_code > 0) {
    if (disk_request_pid[0] == pid) {
      disk_request_pid[0] = -1;
    } else if (disk_request_pid[1] == pid) {
      disk_request_pid[1] = -1;
    }
  }

  return true;
}


/**
 * @brief Verifica se os todos os recusos reqeridos pelo processo estão 
 *        disponíveis. Caso afirmativo, retorna true; caso negativo retorna 
 *        false.
 * 
 * @param process 
 * @return true 
 * @return false 
 */
bool ResourceManager::checkEnoughResources(Process *process)
{
  if (process->isRealtime()) return true;

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

  if (disk_code > 0 
    && disk_request_pid[0] != -1
    && disk_request_pid[1] != -1
  )
    return false;

  return true;
}
