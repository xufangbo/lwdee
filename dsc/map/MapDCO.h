#pragma once

#include <vector>

#include "ToReduce.hpp"
#include "api/UhconnDco.h"

class MapDCO : public UhconnDco {
 public:
  MapDCO();
  virtual ~MapDCO();

 private:
  PartitionMap input;
  ToReduce toReduce;
  std::string start(std::string a);
  std::string map(std::string a);
};