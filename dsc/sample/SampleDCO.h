#pragma once

#include <vector>

#include "api/UhconnDco.h"
#include "lwdee/DDO.h"

class SampleDCO : public UhconnDco {
 public:
  SampleDCO();
  virtual ~SampleDCO();

 private:
  std::string sample(std::string a);

 public:
  static std::vector<DDO> ddos;
};