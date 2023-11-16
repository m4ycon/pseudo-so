#pragma once
#include "./common.h"


// #ifdef _WIN32
//   #include <windows.h> // Windows
//   void sleep(int seconds) {
//     Sleep(seconds * 1e3); // milliseconds
//   }
// #else
//   #include <unistd.h> // Linux
//   void sleep(int seconds) {
//     usleep(seconds * 1e6); // microseconds
//   }
// #endif


class Utils
{
public:
  static void sleep(int time);
  static double randomDouble(double min, double max);
};
