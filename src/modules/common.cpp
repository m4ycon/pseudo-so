#include "../include/common.h"

std::mutex printMtx;
bool DEBUG = false;

void print(const std::string &message)
{
  std::lock_guard<std::mutex> lock(printMtx); // lock is released when 'lock' goes out of scope
  std::cout << message << std::endl;
}

void printd(const std::string &message)
{
  if (DEBUG) {
    std::lock_guard<std::mutex> lock(printMtx); // lock is released when 'lock' goes out of scope
    std::cout << message << std::endl;
  }
}
