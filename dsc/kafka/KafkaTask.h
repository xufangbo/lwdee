#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "core/Partition.h"

typedef vector<string> Strings;
typedef shared_ptr<Strings> Strings_ref;

class KafkaTask {
 private:
  PartitionKafka *partition;

 public:
  std::string start(PartitionKafka *p);

 private:
};
