#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <thread>

#include <librdkafka/rdkafkacpp.h>
#include "core/Partition.h"
#include "lwdee/lwdee.h"

using namespace std;

class ToMap {
 private:
  int window = 10;
  int currentMap = -1;
  vector<DCO> mapDocs;
  vector<vector<string>> mapLines;

 public:
  ToMap();
  void accept(RdKafka::Message* message);
  void create_dco(int size);

 private:
  int nextMap();
  void toMap(int index);
  string json(vector<string> *lines);

 private:
  queue<pair<DDOId, DCO>> ddoIds;
  thread releaseThread;

  void releaseDdo();
};
