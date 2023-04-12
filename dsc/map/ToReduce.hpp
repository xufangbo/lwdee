#pragma once

#include <memory>
#include <thread>
#include <vector>
#include "core/Partition.h"
#include "lwdee/lwdee.h"

using namespace std;

class ToReduce {
 private:
  PartitionMap* input;
  std::vector<DCO> reduceDcos;
  std::hash<std::string> _hash;

 public:
  ~ToReduce() {
    if (ddoIds != nullptr) {
      delete ddoIds;
      ddoIds = nullptr;
    }
  }
  void create_dcos(PartitionMap* input);
  void send(vector<string>* words);

 private:
  list<pair<DDOId, DCO*>>* ddoIds = new list<pair<DDOId, DCO*>>();
  thread releaseThread;

  void releaseDdo();
};