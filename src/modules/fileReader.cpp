#include "../include/fileReader.h"

vector<Process *> FileReader::readProcesses(string path)
{
  string in_startup_time, in_priority, in_processor_time, in_memory_blocks, in_printer_code_requested, in_scanner_request, in_modem_request, in_disk_code;
  int startup_time, priority, processor_time, memory_blocks, printer_code_requested, scanner_request, modem_request, disk_code;

  vector<Process *> processes;
  ifstream file(path);
  string line;

  if (file.is_open()) {
    while (getline(file, line)) {
      stringstream ss(line);
      line.erase(remove(line.begin(), line.end(), ','), line.end()); // remove commas

      ss >> in_startup_time >> in_priority >> in_processor_time >> in_memory_blocks >> in_printer_code_requested >> in_scanner_request >> in_modem_request >> in_disk_code;

      startup_time = stoi(in_startup_time);
      priority = stoi(in_priority);
      processor_time = stoi(in_processor_time);
      memory_blocks = stoi(in_memory_blocks);
      printer_code_requested = stoi(in_printer_code_requested);
      scanner_request = stoi(in_scanner_request);
      modem_request = stoi(in_modem_request);
      disk_code = stoi(in_disk_code);

      auto process = new Process(startup_time, priority, processor_time, memory_blocks, printer_code_requested, scanner_request, modem_request, disk_code);
      processes.push_back(process);
    }
    file.close();
    return processes;
  }

  throw runtime_error("Unable to open file. Path: " + path);
}

vector<File *> FileReader::readFiles(string path)
{
  return vector<File *>();
}
