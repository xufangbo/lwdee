#pragma once

#include "core/Partition.h"

class Step2Task {
 private:
  PartitionStep2* input;
  Step2Output output;

 public:
  Step2Output run(PartitionStep2* p);

 private:
  void reduceByKey(){};
};