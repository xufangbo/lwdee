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
  int mapCount = 0;
  std::vector<string> mapVoxors;

 public:
  PartitionKafka() {}
  PartitionKafka(int index, string group, string topic, std::vector<string> mapVoxors)
      : Partition(index), group(group), topic(topic), mapVoxors(mapVoxors) {}

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

/**
 * 因为lwdee不能返回多个DDO，所以二次获取DDO
 * 这个对象是首次DDO返回的信息明细
 */
typedef struct {
  std::string voxorId;
  DdoDataId dataId;
} SubSplitDDO;

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

class Step2Output {
 public:
  bool succeed;

  std::string toJson();
  void fromJson(std::string* json);
};