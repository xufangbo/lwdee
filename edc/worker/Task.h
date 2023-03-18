#pragma once

#include <iostream>
#include "core/Partition.h"

using namespace std;

class Task {
 protected:
  virtual ByteSpan_ref runTask(Partition *p) = 0;

 public:
  ByteSpan_ref run(Partition *p) { this->runTask(p); };
};