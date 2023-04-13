#pragma once

#include <time.h>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <thread>

#include <librdkafka/rdkafkacpp.h>
#include "core/Partition.h"
#include "lwdee/lwdee.h"

using namespace std;

class ToMap {
 private:
  bool inputFilter = true;
  int currentMap = -1;
  int mapSize = 0;
  PartitionKafka* input;
  vector<DCO> mapDocs;
  vector<vector<MapRecord>>* mapLines = new vector<vector<MapRecord>>();

 public:
  ToMap();
  ~ToMap();
  void accept(RdKafka::Message* message);
  void create_dco(PartitionKafka* input);

 private:
  uint64_t currentTs;
  std::mutex mut;
  int nextMap();
  void toMaps();
  void toMap(int index);

 private:
  list<pair<DDOId, DCO*>>* ddoIds = new list<pair<DDOId, DCO*>>();
  thread releaseThread;

  void releaseDdo();
};
