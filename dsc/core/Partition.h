#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Partition {
 public:
  Partition() {}
  Partition(int index) : index(index) {}
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
  PartitionKafka(int index, string group, string topic, int window,
                 std::vector<string> mapVoxors)
      : Partition(index),
        group(group),
        topic(topic),
        window(window),
        mapVoxors(mapVoxors) {}

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
  PartitionReduce(int index) : Partition(index) {}

  std::string toJson();
  void fromJson(std::string* json);
};

struct MapRecord {
  std::string line;
  double ts;

  MapRecord() {}
  MapRecord(std::string line, double ts) : line(line), ts(ts) {}

  bool fromJson(std::string* json);
};

class MapInvokeData {
 public:
  int kafkaIndex;
  int mapIndex;
  vector<MapRecord>* items = nullptr;

 public:
  MapInvokeData() {}
  MapInvokeData(int kafkaIndex,int mapIndex, vector<MapRecord>* items)
      : kafkaIndex(kafkaIndex),mapIndex(mapIndex), items(items) {}

  std::string toJson();
  void fromJson(std::string* json);
};

struct ReduceRecord {
  std::string did;
  double ts;

  bool fromJson(std::string* json);
};

class ReduceInvokeData {
 public:
  int mapIndex;
  int reduceIndex;
  vector<ReduceRecord>* items = nullptr;

 public:
  ReduceInvokeData() {}
  ReduceInvokeData(int mapIndex, int reduceIndex,vector<ReduceRecord>* items)
      : mapIndex(mapIndex),reduceIndex(reduceIndex), items(items) {}

  std::string toJson();
  void fromJson(std::string* json);
};

class StringsSerializer {
 public:
  static std::string toJson(int index, vector<string>* items);
  static int fromJson(std::string& json, vector<string>* items);
};

class ServicePaths {
 public:
  static std::string kafka_start;
  static std::string map_start;
  static std::string map_invoke;
  static std::string reduce_start;
  static std::string reduce_invoke;
};