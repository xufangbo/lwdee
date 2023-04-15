#pragma once

#include <vector>

#include "core/Partition.h"

class KafkaDCO {
 private:
  static PartitionKafka input;

 public:
  static std::string start(std::string a);
};

void regist_kafka_start_service();