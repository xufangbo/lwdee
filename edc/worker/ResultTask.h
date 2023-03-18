#pragma once
#include "Task.h"

class ResultTask : public Task {
 private:
  ByteSpan_ref runTask(Partition *p);
  void reduceByKey();
};