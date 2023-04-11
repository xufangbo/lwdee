#pragma once

#include "api/UhconnDco.h"
#include "core/Partition.h"
#include "Reducer.hpp"

class ReduceDCO : public UhconnDco {
 public:
  ReduceDCO();
  virtual ~ReduceDCO();

 private:
  Reducer reducer;
  PartitionReduce input;
  std::string start(std::string a);
  std::string reduce(std::string a);
};