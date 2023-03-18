#pragma once
#include <iostream>

#include "lwdee/DDO.h"

using namespace std;

class Partition {
 public:
  Partition() {}
  Partition(int index) : index(index) {}
  int index;
};

class PartitionInput : public Partition {
 public:
  PartitionInput() {}
  PartitionInput(int index, string fileName)
      : Partition(index), fileName(fileName) {}
  string fileName;
};

class PartitionStage : public Partition {
 public:
  PartitionStage() {}
  PartitionStage(int index, DDO ddo) : Partition(index), ddo(ddo) {}
  DDO ddo;
};