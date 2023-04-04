#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "core/Partition.h"
#include "core/TeraRecord.hpp"

class Step0Task {
 private:
  PartitionStep0* partition;

 public:
  MinAndMax run(PartitionStep0* p);

 private:
  /**
   * 采样得到最大值最小值
   * */
  MinAndMax samples(std::string fileName);
};
