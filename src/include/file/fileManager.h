#pragma once
#include "../common.h"
#include "./file.h"
#include "./fileInstruction.h"

class FileManager {
  public:
    FileManager();

    void setup(int disk_size, int occuped_blocks, vector<File *> files, vector<FileInstruction *> instructions);
  private:
};
