#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "ToReduce.hpp"
#include "core/Partition.h"

typedef vector<string> Strings;
typedef shared_ptr<Strings> Strings_ref;

class Mapper {
 public:
  static void map(std::vector<MapRecord>* lines, vector<ReduceRecord>* words);
};
