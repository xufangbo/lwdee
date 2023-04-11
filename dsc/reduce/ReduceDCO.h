#pragma once

#include "api/UhconnDco.h"
#include "core/Partition.h"

class ReduceDCO : public UhconnDco {
 public:
  ReduceDCO();
  virtual ~ReduceDCO();

 private:
  PartitionReduce input;
  std::string start(std::string a);
  std::string reduce(std::string a);
};