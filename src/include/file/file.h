#pragma once
#include "../common.h"

class File {
  public:
    File(char filename, int start_block, int size);

    char filename;
    int start_block;
    int size;
  private:
};
