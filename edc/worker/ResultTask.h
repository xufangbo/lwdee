#pragma once
#include "Task.h"

class ResultTask : public Task {
 private:
  DDO runTask(Partition *p);
  void reduceByKey();
};