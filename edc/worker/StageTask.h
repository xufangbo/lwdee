#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Task.h"
#include "core/TuplesSerialzer.h"

typedef vector<string> Strings;
typedef shared_ptr<Strings> Strings_ref;

class StageTask : public Task {
 private:
  DDO runTask(Partition *p);

  Strings_ref textFile(PartitionInput *p);
  Strings_ref flatMap(string line);
  Tuple map(string word);  
};
