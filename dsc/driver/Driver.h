#pragma once

#include <iostream>
#include <vector>

#include "core/DscConfig.hpp"
#include "core/Partition.h"
#include "lwdee/DCO.h"

class Driver {
 private:
  DscConfig* conf = nullptr;
  std::vector<pair<DCO, DDOId>> kafkaInvokers;
  std::vector<pair<DCO, DDOId>> mapInvokers;
  std::vector<pair<DCO, DDOId>> reduceInvokers;
  std::vector<PartitionStep2> step2Inputs;

 public:
  void startJob();

 private:
  void kafka();
  void kafkaToMap();

  void map();

  void mapToReduce();

  /**
   * 执行reduce计算
   * */
  void reduce();
};