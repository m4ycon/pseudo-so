#pragma once
#include "../common.h"
#include "./file.h"
#include "../process/instruction.h"

enum FileActionCode
{
  OK,
  FILE_NOT_FOUND,
  NOT_ENOUGH_SPACE,
};

const string EMPTY_BLOCK = " ";

class FileManager
{
public:
  FileManager(int disk_size, vector<File *> files);

  FileActionCode addFile(File *file);
  FileActionCode removeFile(string filename);
  File* getFile(string filename);

  void printDisk();

private:
  int disk_size, remaining_size;
  string disk_blocks[10000];

  void compactDisk();
  int getContiguousIndex(int size);
};
