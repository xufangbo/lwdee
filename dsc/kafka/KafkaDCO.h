#pragma once

#include <vector>

#include "api/UhconnDco.h"
#include "lwdee/DDO.h"

class KafkaDCO : public UhconnDco {
 public:
  KafkaDCO();
  virtual ~KafkaDCO();

 private:
  std::string start(std::string a);
  std::string ddo(std::string voxorId,DdoDataId ddoId);

 public:
  static std::vector<DDO> ddos;
};