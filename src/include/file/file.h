#pragma once
#include "../common.h"

class File {
  public:
    File(string filename, int start_block, int size);

  private:
    string filename;
    int start_block;
    int size;
};
