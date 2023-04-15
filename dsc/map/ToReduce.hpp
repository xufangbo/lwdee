#pragma once

#include <memory>
#include <thread>
#include <vector>
#include "core/Partition.h"
#include "core/NodeConfig.hpp"

using namespace std;

class ToReduce {
 private:
  PartitionMap* input;
  std::vector<VoxorId> reduceDcos;
  std::hash<std::string> _hash;

 public:
  void create_dcos(PartitionMap* input);
  void send(vector<ReduceRecord>* words);
};