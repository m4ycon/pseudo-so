#include "../include/utils.h"


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

#ifdef _WIN32
  #include <windows.h> // Windows
  void Utils::sleep(double seconds)
  {
    printd("Utils::sleep()");
    Sleep(seconds * 1e3); // milliseconds
  }
#else
  #include <unistd.h> // Linux
  void Utils::sleep(double seconds)
  {
    printd("Utils::sleep()");
    usleep(seconds * 1e6); // microseconds
  }
#endif

double Utils::randomDouble(double min, double max)
{
  std::random_device rd;  // Create a random device
  std::mt19937 gen(rd()); // Create a Mersenne Twister pseudo-random number generator
  std::uniform_real_distribution<> dis(min, max);

  return dis(gen);
}
