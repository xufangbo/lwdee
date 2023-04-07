#pragma once

#include <vector>

#include "ToReduce.hpp"
#include "api/UhconnDco.h"

class MapDCO : public UhconnDco {
 public:
  MapDCO();
  virtual ~MapDCO();

 private:
  ToReduce toReduce;
  std::string accept(std::string a);
};