#pragma once

#include <vector>

#include "api/UhconnDco.h"
#include "lwdee/DDO.h"

class MapDCO : public UhconnDco {
 public:
  MapDCO();
  virtual ~MapDCO();

 private:
  std::string map(std::string a);
  std::string ddo(std::string voxorId,DdoDataId ddoId);

 private:
  static std::vector<DDO> ddos;
};