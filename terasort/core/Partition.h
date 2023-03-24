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

class PartitionStep1 : public Partition {
 public:
  string fileName;
  int outSplitNums;
  vector<DDO> ddos;

 public:
  PartitionStep1() {}
  PartitionStep1(int index, string fileName,int outSplitNums)
      : Partition(index), fileName(fileName),outSplitNums(outSplitNums) {}
};

class PartitionStep2 : public Partition {
 public:
  vector<DDO> ddos;

 public:
  PartitionStep2() {}
  PartitionStep2(int index)
      : Partition(index){}
};