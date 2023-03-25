#pragma once
#include <iostream>
#include <vector>

#include "lwdee/DDO.h"

using namespace std;

class Partition {
 public:
  Partition() {}
  Partition(int index) : index(index) {}
  int index;
};

class SampleSplit {
 public:
  uint64_t min;
  uint64_t max;
};

class PartitionStep1 : public Partition {
 public:
  string fileName;
  vector<SampleSplit> sampleSplits;
  vector<DDO> ddos;

 public:
  PartitionStep1() {}
  PartitionStep1(int index, string fileName, vector<SampleSplit> sampleSplits)
      : Partition(index), fileName(fileName), sampleSplits(sampleSplits) {}
};

class PartitionStep2 : public Partition {
 public:
  vector<DDO> ddos;

 public:
  PartitionStep2() {}
  PartitionStep2(int index) : Partition(index) {}
};