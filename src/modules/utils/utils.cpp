#include "../../include/utils/utils.h"


std::mutex printMtx;
bool DEBUG = false;

void print(const std::string &message, bool endline)
{
  std::lock_guard<std::mutex> lock(printMtx); // lock is released when 'lock' goes out of scope
  std::cout << message;
  if (endline) std::cout << std::endl;
}

#ifdef _WIN32
  #include <windows.h> // Windows
  void Utils::sleep(double seconds)
  {
    Sleep(seconds * 1e3); // milliseconds
  }
#else
  #include <unistd.h> // Linux
#include "utils.h"
  void Utils::sleep(double seconds)
  {
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

void Utils::removeCommas(std::string &str)
{
  str.erase(remove(str.begin(), str.end(), ','), str.end());
}

TimePoint Utils::now()
{
  return std::chrono::steady_clock::now();
}

int Utils::getElapsedTime(TimePoint startTime)
{
  auto endTime = Utils::now();
  auto elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

  return elapsedTimeMs;
}
