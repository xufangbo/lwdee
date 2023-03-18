#pragma once

#include <iostream>
#include "core/Partition.h"

using namespace std;

class Task {
 protected:
  virtual DDO runTask(Partition *p) = 0;

 public:
  DDO run(Partition *p) { this->runTask(p); };
};