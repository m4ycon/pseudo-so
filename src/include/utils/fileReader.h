#pragma once
#include <fstream>
#include <sstream>
#include <algorithm>

#include "../common.h"
#include "../file/file.h"
#include "../file/fileInstruction.h"
#include "../file/fileManager.h"
#include "../process/process.h"
#include "../scheduler/scheduler.h"

struct FilesInfo
{
  int disk_size, segment_blocks;
  vector<File *> files;
  vector<FileInstruction *> instructions;
};

class FileReader
{
public:
  static FileManager* setup(string processes_path, string files_path, Scheduler *scheduler);

private:
  static vector<Process*> readProcesses(string path);
  static FilesInfo readFiles(string path);
};