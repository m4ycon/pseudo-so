#include "../../include/utils/fileReader.h"

pair<FileManager*, vector<Process*>> FileReader::setup(string processes_path, string files_path, Scheduler *scheduler)
{
  auto processes = readProcesses(processes_path);
  auto files_info = readFiles(files_path, processes);
  
  auto fileManager = new FileManager(files_info.disk_size, files_info.files);

  return { fileManager, processes };
}

vector<Process *> FileReader::readProcesses(string path)
{
  string in_startup_time, in_priority, in_processor_time, in_memory_blocks, in_printer_code_requested, in_scanner_request, in_modem_request, in_disk_code;
  int startup_time, priority, processor_time, memory_blocks, printer_code_requested, scanner_request, modem_request, disk_code;

  vector<Process *> processes;
  ifstream file(path);
  string line;

  if (file.is_open()) {
    while (getline(file, line)) {
      Utils::removeCommas(line);
      stringstream ss(line);

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

FilesInfo FileReader::readFiles(string path, vector<Process*> processes)
{
  vector<File *> files;

  string input_aux;
  int disk_size, segment_blocks;
  
  ifstream file(path);
  string line;

  if (file.is_open()) {
    // basic info
    getline(file, line);
    stringstream ss(line);
    ss >> input_aux, disk_size = stoi(input_aux);

    getline(file, line);
    Utils::removeCommas(line), ss.clear(), ss.str(line);
    ss >> input_aux, segment_blocks = stoi(input_aux);

    // files
    for(int i = 0; i < segment_blocks; i++) {
      getline(file, line);
      Utils::removeCommas(line), ss.clear(), ss.str(line);

      char filename;
      int start_block, size;

      ss >> filename;
      ss >> input_aux, start_block = stoi(input_aux);
      ss >> input_aux, size = stoi(input_aux);

      files.push_back(new File(filename, start_block, size));
    }

    // file manager operations
    while (getline(file, line)) {
      Utils::removeCommas(line), ss.clear(), ss.str(line);

      int pid, opcode, numBlocks = 0;
      char filename;

      ss >> input_aux, pid = stoi(input_aux);
      ss >> input_aux, opcode = stoi(input_aux);
      ss >> filename;
      if (opcode == 0) ss >> input_aux, numBlocks = stoi(input_aux);

      auto instruction = new Instruction(pid, opcode, filename, numBlocks);
      if (pid < (int) processes.size())
        processes[pid]->addInstruction(instruction);
    }
    file.close();

    return { disk_size, files };
  }

  throw runtime_error("Unable to open file. Path: " + path);
}
