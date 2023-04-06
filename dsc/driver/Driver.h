#pragma once

#include <iostream>
#include <vector>

#include "core/Partition.h"
#include "lwdee/DCO.h"
#include "core/DscConfig.hpp"

class Driver {
 private:
  DscConfig* conf = nullptr;
  std::vector<pair<DCO, DDOId>> step1Invokers;
  std::vector<pair<DCO, DDOId>> step2Invokers;
  std::vector<PartitionStep2> step2Inputs;

 public:
  void startJob();

 private:

  /**
   * 执行map计算
   * */
  void map();

  void mapToReduce();

  /**
   * 执行reduce计算
   * */
  void reduce();
};