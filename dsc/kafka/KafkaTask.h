#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "core/Partition.h"

typedef vector<string> Strings;
typedef shared_ptr<Strings> Strings_ref;

class KafkaTask {
 private:
  PartitionStep1 *partition;

 public:
  std::string start(PartitionStep1 *p);

 private:
};
