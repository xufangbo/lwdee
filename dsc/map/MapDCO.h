#pragma once

#include <vector>

#include "ToReduce.hpp"

class MapDCO {
 private:
  static PartitionMap input;
  static ToReduce toReduce;

 public:
  static std::string start(std::string a);
  static std::string map(std::string a);
};

void regist_map_start_service();
void regist_map_invoke_service();