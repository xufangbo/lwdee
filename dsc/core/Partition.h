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

 public:
  PartitionKafka() {}
  PartitionKafka(int index, string group, string topic)
      : Partition(index), group(group),topic(topic) {}

  std::string toJson();
  void fromJson(std::string* json);
};


class PartitionMap : public Partition {
 public:
  string fileName;
  DDO outputDDO;

 public:
  PartitionMap() {}
  PartitionMap(int index, string fileName)
      : Partition(index), fileName(fileName) {}

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
  vector<SubSplitDDO> subSplitDDOs;

 public:
  PartitionReduce() {}
  PartitionReduce(int index, string outputFile)
      : Partition(index), outputFile(outputFile) {}

  std::string toJson();
  void fromJson(std::string* json);
};

class Step2Output {
 public:
  bool succeed;

  std::string toJson();
  void fromJson(std::string* json);
};