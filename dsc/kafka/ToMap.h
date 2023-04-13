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
  int filterCondon = 100;
  int mapSize = 0;
  PartitionKafka* input;
  vector<DCO> mapDocs;
  vector<MapRecord>* mapRecords = new vector<MapRecord>();

 public:
  ToMap();
  ~ToMap();
  void accept(RdKafka::Message* message);
  void push(RdKafka::Message* message);
  void create_dco(PartitionKafka* input);

 private:
  uint64_t currentWindowTs = 0;
  uint64_t window = 1000;
  std::mutex mut;
  uint64_t getCurrentWindow();
  void toMaps();
  void toMap(int index,vector<MapRecord>* mapLines);

 private:
  list<pair<DDOId, DCO*>>* ddoIds = new list<pair<DDOId, DCO*>>();
  thread releaseThread;

  void releaseDdo();
};
