#pragma once

#include <iostream>
#include <vector>

#include "core/DscConfig.hpp"
#include "core/Partition.h"
#include "lwdee/DCO.h"

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
  void get_ddo(std::string message, int index, size_t size, DCO& dco, DDOId& ddoId);
};