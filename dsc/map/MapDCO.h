#pragma once

#include <vector>

#include "ToReduce.hpp"

class MapDCO {
 private:
  PartitionMap input;
  ToReduce toReduce;

 public:
  std::string start(std::string a);
  std::string map(std::string a);
};

void regist_map_start_service();
void regist_map_invoke_service();