#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "core/MapSerializer.h"
#include "core/Partition.h"
#include "core/TuplesSerialzer.h"

typedef vector<string> Strings;
typedef shared_ptr<Strings> Strings_ref;

class Step1Task {
 private:
  PartitionStep1 *partition;

 public:
  PartitionStep1 run(PartitionStep1 *p);

 private:
  Strings_ref textFile(PartitionStep1 *p);
  Strings_ref flatMaps(Strings_ref lines);
  Strings_ref flatMap(string line);
  Tuples_ref maps(Strings_ref words);
  Tuple map(string word);
  void groupByKey(Tuples *tuples);
};
