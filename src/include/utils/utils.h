#pragma once
#include <random>
#include <mutex>
#include <iostream>
#include <algorithm>

class Utils
{
public:
  static void sleep(double time);
  static double randomDouble(double min, double max);
  static void removeCommas(std::string &str);
};

void print(const std::string &message);
void printd(const std::string &message);
