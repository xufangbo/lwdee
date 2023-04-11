#pragma once

#include <vector>

#include "api/UhconnDco.h"
#include "lwdee/DDO.h"
#include "core/Partition.h"

class KafkaDCO : public UhconnDco {
 public:
  KafkaDCO();
  virtual ~KafkaDCO();

 private:
 PartitionKafka input;
  std::string start(std::string a);

 public:
  static std::vector<DDO> ddos;
};