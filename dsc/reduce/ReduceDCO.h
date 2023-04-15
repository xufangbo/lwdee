#pragma once

#include "Reducer.hpp"
#include "core/Partition.h"

class ReduceDCO {

 private:
  static Reducer reducer;
  static PartitionReduce input;

 public:
  static std::string start(std::string a);
  static std::string reduce(std::string a);
};

void regist_reduce_start_service();
void regist_reduce_invoke_service();