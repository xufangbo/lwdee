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
  void startKafka();

  void startMap();

  /**
   * 执行reduce计算
   * */
  void startReduce();
};