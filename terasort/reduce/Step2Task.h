#pragma once

#include "core/Partition.h"

class Step2Task{
public:
  DDO run(PartitionStep2 *p);
   private:
  void reduceByKey();
};