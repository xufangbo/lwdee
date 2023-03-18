#pragma once
#include "Task.h"

class ResultTask : public Task {
 private:
  void runTask(Partition *p);
  void reduceByKey();
};