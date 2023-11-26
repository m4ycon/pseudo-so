#pragma once
#include <fstream>
#include <sstream>
#include <algorithm>

#include "./common.h"
#include "./process.h"
#include "./file.h"
#include "./instruction.h"
#include "./scheduler.h"
#include "./fileManager.h"

struct FilesInfo
{
  int disk_size, occuped_blocks;
  vector<File *> files;
  vector<Instruction *> instructions;
};

class FileReader
{
public:
  static void setup(string processes_path, string files_path, Scheduler *scheduler, FileManager *fileManager);

private:
  static vector<Process *> readProcesses(string path);
  static FilesInfo readFiles(string path);
};
