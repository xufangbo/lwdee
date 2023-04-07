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

 public:
  static std::vector<DDO> ddos;
};