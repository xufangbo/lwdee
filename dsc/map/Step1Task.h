#pragma once

#include <functional>
#include <memory>
#include <vector>

// #include "core/MapSerializer.h"
#include "core/Partition.h"
#include "core/TeraRecord.hpp"
// #include "core/TuplesSerialzer.h"

typedef vector<string> Strings;
typedef shared_ptr<Strings> Strings_ref;

class Step1Task {
 private:
  PartitionStep1 *partition;
  Step1Output output;

 public:
  Step1Output run(PartitionStep1 *p);

 private:
  TeraRecords* textFile();
  void generateSubSplit(TeraRecords *trs);
  int classify(TeraRecord &tr);
};
