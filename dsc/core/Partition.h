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

class PartitionStep1 : public Partition {
 public:
  string fileName;
  DDO outputDDO;

 public:
  PartitionStep1() {}
  PartitionStep1(int index, string fileName)
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

class Step1Output {
 public:
  std::vector<SubSplitDDO> items;

  std::string toJson();
  void fromJson(std::string* json);
};

class PartitionStep2 : public Partition {
 public:
  string outputFile;
  vector<SubSplitDDO> subSplitDDOs;

 public:
  PartitionStep2() {}
  PartitionStep2(int index, string outputFile)
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