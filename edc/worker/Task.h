#pragma once

#include <iostream>
#include "core/Partition.h"

using namespace std;

class Task {
 protected:
  virtual void runTask(Partition *p) = 0;

 public:
  void run(Partition *p) { this->runTask(p); };
};