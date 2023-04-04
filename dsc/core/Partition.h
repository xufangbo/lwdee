#pragma once
#include <iostream>
#include <vector>
#include "lwdee/DDO.h"
#include "Bytes10.h"

using namespace std;

typedef std::pair<Bytes10, Bytes10> MinAndMax;

class Partition {
 public:
  Partition() {}
  Partition(int index)
      : index(index) {}
  int index;
};

class SampleSplit {
 public:
  uint64_t min;
  uint64_t max;
};

class PartitionStep0 : public Partition {
 public:
  string fileName;
  int splitNums1;
  int datum;
  DDO outputDDO;

 public:
  PartitionStep0() {}
  PartitionStep0(int index, string fileName,int splitNums1,int datum)
      : Partition(index), fileName(fileName),splitNums1(splitNums1),datum(datum) {}

  std::string toJson();
  void fromJson(std::string* json);
};

class PartitionStep1 : public Partition {
 public:
  string fileName;
  vector<SampleSplit> sampleSplits;
  DDO outputDDO;

 public:
  PartitionStep1() {}
  PartitionStep1(int index, string fileName, vector<SampleSplit> sampleSplits)
      : Partition(index), fileName(fileName), sampleSplits(sampleSplits) {}

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