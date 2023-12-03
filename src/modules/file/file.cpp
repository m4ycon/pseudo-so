#include "../../include/file/file.h"

/**
 * @brief Construct a new File:: File object
 * 
 * @param filename 
 * @param start_block 
 * @param size 
 */
File::File(char filename, int start_block, int size)
{
  this->filename = filename;
  this->start_block = start_block;
  this->size = size;
}
