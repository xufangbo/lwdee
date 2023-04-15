#pragma once

#include <iostream>
#include <vector>

#include "core/DscConfig.hpp"
#include "core/Partition.h"

class Driver {
 private:
  DscConfig* conf = nullptr;
  std::vector<std::string> mapVoxorIds;
  std::vector<std::string> reduceVoxorIds;

 public:
  void startJob();

 private:
  void start_kafka();
  void start_map();
  void start_reduce();
};