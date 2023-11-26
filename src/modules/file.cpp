#include "../include/file.h"

File::File(string filename, int start_block, int size)
{
  this->filename = filename;
  this->start_block = start_block;
  this->size = size;
}
