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
  std::string group;
  std::string topic;
  std::string outTopic;
  std::vector<int> partitions;
  int splitNums1 = 0;
  int splitNums2 = 0;
  std::string partitionsString();

 public:
  void readConfig();
  static DscConfig* instance();
};
