#pragma once

#include <vector>

#include "api/UhconnDco.h"
#include "lwdee/lwdee.h"

class MapDCO : public UhconnDco {
 public:
  MapDCO();
  virtual ~MapDCO();

 private:
  std::string accept(std::string a);

 private:
  static std::vector<DCO> reduceDcos;
};