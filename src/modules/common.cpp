#include "../include/common.h"

mutex printMtx;
void print(const string &message)
{
  lock_guard<mutex> lock(printMtx); // lock is released when 'lock' goes out of scope
  cout << message << endl;
}
