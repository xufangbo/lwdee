#pragma once

#include <vector>
#include <memory>
#include "ToReduce.hpp"

class MapDCO {
 private:
  PartitionMap input;
  ToReduce toReduce;

 public:
  int start(std::string a);
  std::string map(std::shared_ptr<vector<MapRecord>> lines);
};

