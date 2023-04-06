#pragma once

#include <iostream>
#include <vector>

class DscConfig {
 private:
  static DscConfig _instance;

 public:
  // terasort
  std::string inputFile;
  std::string brokers;
  std::string topic;
  std::vector<int> partitions;
  int splitNums1 = 0;
  int splitNums2 = 0;

 public:
  void readConfig();
  static DscConfig* instance();
};
