#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include <librdkafka/rdkafkacpp.h>
#include "core/Partition.h"
#include "lwdee/lwdee.h"

using namespace std;

class ToMap {
 private:
  vector<DCO> mapDocs;
  vector<vector<string>> mapLines;

 public:
  void accept(RdKafka::Message* message);
  void create_dco(int size);

 private:
  int currentMap = -1;
  int nextMap();
  void toMap(int index);

 private:
  int window = 10;
};
