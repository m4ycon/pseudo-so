#pragma once
#include <fstream>
#include <sstream>
#include <algorithm>

#include "../common.h"
#include "../file/file.h"
#include "../process/instruction.h"
#include "../file/fileManager.h"
#include "../process/process.h"
#include "../scheduler/scheduler.h"

struct FilesInfo
{
  int disk_size;
  vector<File *> files;
};

class FileReader
{
public:
  static pair<FileManager*, vector<Process*>> setup(string processes_path, string files_path, Scheduler *scheduler);

private:
  static vector<Process*> readProcesses(string path);
  static FilesInfo readFiles(string path, vector<Process*> processes);
};
