#pragma once

#include <thread>
#include <vector>
#include "core/Partition.h"
#include "lwdee/lwdee.h"

using namespace std;

class ToReduce {
 private:
  std::vector<DCO> reduceDcos;
  std::hash<std::string> _hash;

 public:
  void create_dcos(PartitionMap* input);
  void send(vector<string>& words);

 private:
 
  list<pair<DDOId, DCO*>> ddoIds;
  thread releaseThread;

  void releaseDdo();
};