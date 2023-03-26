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
  DDO outputDDO;

 public:
  PartitionStep1() {}
  PartitionStep1(int index, string fileName, vector<SampleSplit> sampleSplits)
      : Partition(index), fileName(fileName), sampleSplits(sampleSplits) {}
};

/**
 * 因为lwdee不能返回多个DDO，所以二次获取DDO
 * 这个对象是首次DDO返回的信息明细
 */
typedef struct {
  std::string voxorId;
  DdoDataId dataId;
} Step1ResultDDOItem;

typedef struct {
  std::vector<Step1ResultDDOItem> items;

  int totoalSize() {
    int size = 0;
    size += 4;  // items count
    for (Step1ResultDDOItem &item : items) {
      size += 4;  // voxorId size
      size += item.voxorId.size();
      size += sizeof(item.dataId);
    }
  }

  ByteSpan_ref serialize() {
    ByteSpan_ref bytes = std::make_shared<ByteSpan>(this->totoalSize());
    bytes->putInt32(items.size());
    for (Step1ResultDDOItem &item : items) {
      bytes->putInt32(item.voxorId.size());
      bytes->puts((Byte *)item.voxorId.data(), item.voxorId.size());
      bytes->puts((Byte *)&item.dataId, sizeof(item.dataId));
    }
  };
  void deserialize(ByteSpan *bytes) {
    int itemsCount = 0;
    bytes->readInt32(itemsCount);

    for (int i = 0; i < itemsCount; i++) {
      Step1ResultDDOItem item;
      int voxorSize = 0;
      bytes->readInt32(voxorSize);

      item.voxorId = std::string(voxorSize + 1, '\0');
      bytes->reads((Byte *)item.voxorId.data(), voxorSize);

      bytes->reads((Byte *)&item.dataId, sizeof(item.dataId));
    }
  };

} Step1ResultDDO;

class PartitionStep2 : public Partition {
 public:
  vector<DDO> ddos;

 public:
  PartitionStep2() {}
  PartitionStep2(int index) : Partition(index) {}
};