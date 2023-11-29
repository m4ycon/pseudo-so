#pragma once
#include <random>
#include <mutex>
#include <iostream>
#include <algorithm>
#include <chrono>

typedef std::chrono::steady_clock::time_point TimePoint;

class Utils
{
public:
  static void sleep(double time);
  static double randomDouble(double min, double max);
  static void removeCommas(std::string &str);
  static int getElapsedTime(TimePoint startTime);
};

void print(const std::string &message = "", bool endl = true);
void printd(const std::string &message = "", bool endl = true);
