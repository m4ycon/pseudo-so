#pragma once
#include "./common.h"
#include <windows.h> // Windows

// #ifdef _WIN32
//   #include <windows.h> // Windows
//   Sleep(seconds * 1e3); // milliseconds
// #else
//   #include <unistd.h> // Linux
//   usleep(seconds * 1e6); // microseconds
// #endif

class Utils
{
public:
  static void sleep(double time);
  static double randomDouble(double min, double max);
};
