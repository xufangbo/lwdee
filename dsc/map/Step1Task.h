#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "core/Partition.h"

typedef vector<string> Strings;
typedef shared_ptr<Strings> Strings_ref;

class Step1Task {
 private:
  PartitionMap *partition;

 public:
  std::string start(PartitionMap *p);

 private:
};
