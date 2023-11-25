#pragma once
#include <fstream>
#include <sstream>
#include <algorithm>

#include "./common.h"
#include "./process.h"
#include "./file.h"


class FileReader {
  public:
  
    static vector<Process*> readProcesses(string path);
    static vector<File*> readFiles(string path);
  private:
};
