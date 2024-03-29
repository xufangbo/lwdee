#pragma once

#include <iostream>
#include <vector>

class DscConfig {
 private:
  static DscConfig _instance;

 public:
  std::string name;
  std::string nodeName;
  std::string inputFile;
  std::string outputFile;
  std::string brokers;
  std::string group;
  std::string topic;
  std::string outTopic;
  int window = 10;
  bool isMatrix = false;
  int filterCondon = 100;
  std::vector<int> partitions;
  int splitNums1 = 0;
  int splitNums2 = 0;
  std::string partitionsString();

 public:
  void readConfig();
  static DscConfig* instance();
};
