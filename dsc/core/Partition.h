#pragma once
#include <iostream>
#include <vector>
#include "lwdee/DDO.h"

using namespace std;

class Partition {
 public:
  Partition() {}
  Partition(int index)
      : index(index) {}
  int index;
};

class PartitionKafka : public Partition {
 public:
  string group;
  string topic;
  int window = 0;
  std::vector<string> mapVoxors;

 public:
  PartitionKafka() {}
  PartitionKafka(int index, string group, string topic, int window, std::vector<string> mapVoxors)
      : Partition(index), group(group), topic(topic), window(window), mapVoxors(mapVoxors) {}

  std::string toJson();
  void fromJson(std::string* json);
};

class PartitionMap : public Partition {
 public:
  std::vector<string> reduceVoxors;

 public:
  PartitionMap() {}
  PartitionMap(int index, std::vector<string> reduceVoxors)
      : Partition(index), reduceVoxors(reduceVoxors) {}

  std::string toJson();
  void fromJson(std::string* json);
};

class PartitionReduce : public Partition {
 public:
  string outputFile;

 public:
  PartitionReduce() {}
  PartitionReduce(int index)
      : Partition(index) {}

  std::string toJson();
  void fromJson(std::string* json);
};

struct DeviceRecord {
  std::string did;
  int ts;
};

class ReduceData {
 public:
  int mapIndex;
  vector<DeviceRecord>* items = nullptr;

 public:
  ReduceData() {}
  ReduceData(int mapIndex, vector<DeviceRecord>* items)
      : mapIndex(mapIndex), items(items) {}

  std::string toJson();
  void fromJson(std::string* json);
};

class StringsSerializer {
 public:
  static std::string toJson(int index, vector<string>* items);
  static int fromJson(std::string& json, vector<string>* items);
};