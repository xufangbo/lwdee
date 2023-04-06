#pragma once

#include <vector>

#include "api/UhconnDco.h"
#include "lwdee/DDO.h"

class MapDCO : public UhconnDco {
 public:
  MapDCO();
  virtual ~MapDCO();

 private:
  std::string accept(std::string a);

 public:
  static std::vector<DDO> ddos;
};