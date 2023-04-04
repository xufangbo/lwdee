#pragma once

#include <vector>

#include "api/UhconnDco.h"
#include "lwdee/DDO.h"

class MapDCO : public UhconnDco {
 public:
  MapDCO();
  virtual ~MapDCO();

 private:
  std::string f1(std::string a);
  std::string map(std::string a);
  std::string ddo(std::string voxorId,DdoDataId ddoId);

 public:
  static std::vector<DDO> ddos;
};