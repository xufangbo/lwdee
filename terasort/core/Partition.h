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

  // int totoalSize() {
  //   int size = 0;
  //   size += 4;                // index
  //   size += 4;                // fileName count
  //   size += fileName.size();  // fileName size
  //   size += 4;                // sub split count

  //   for (SampleSplit& item : sampleSplits) {
  //     size += 8;  // min
  //     size += 8;  // max
  //   }

  //   return size;
  // }

  // ByteSpan_ref serialize() {
  //   ByteSpan_ref bytes = std::make_shared<ByteSpan>(this->totoalSize());
  //   bytes->putInt32(this->index);
  //   bytes->putInt32(this->fileName.size());
  //   bytes->puts((Byte*)this->fileName.data(), fileName.size());

  //   bytes->putInt32(sampleSplits.size());
  //   for (SampleSplit& item : sampleSplits) {
  //     bytes->puts((Byte*)&item.min, sizeof(item.min));
  //     bytes->puts((Byte*)&item.max, sizeof(item.max));
  //   }
  //   return bytes;
  // };
  // void deserialize(ByteSpan* bytes) {
  //   bytes->readInt32(this->index);

  //   int fileNameSize = 0;
  //   bytes->readInt32(fileNameSize);
  //   this->fileName = std::string(fileNameSize + 1, '\0');
  //   bytes->reads((Byte*)fileName.data(), fileNameSize);

  //   int itemsCount = 0;
  //   bytes->readInt32(itemsCount);

  //   for (int i = 0; i < itemsCount; i++) {
  //     SampleSplit item;
  //     bytes->reads((Byte*)item.min, sizeof(item.min));
  //     bytes->reads((Byte*)item.max, sizeof(item.max));

  //     this->sampleSplits.push_back(item);
  //   }
  // }

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

  // int totoalSize() {
  //   int size = 0;
  //   size += 4;  // items count
  //   for (SubSplitDDO &item : items) {
  //     size += 4;  // voxorId size
  //     size += item.voxorId.size();
  //     size += sizeof(item.dataId);
  //   }
  //   return size;
  // };

  // ByteSpan_ref serialize() {
  //   ByteSpan_ref bytes = std::make_shared<ByteSpan>(this->totoalSize());
  //   bytes->putInt32(items.size());
  //   for (SubSplitDDO &item : items) {
  //     bytes->putInt32(item.voxorId.size());
  //     bytes->puts((Byte *)item.voxorId.data(), item.voxorId.size());
  //     bytes->puts((Byte *)&item.dataId, sizeof(item.dataId));
  //   }
  //   return bytes;
  // }

  // void deserialize(ByteSpan *bytes) {
  //   int itemsCount = 0;
  //   bytes->readInt32(itemsCount);

  //   for (int i = 0; i < itemsCount; i++) {
  //     SubSplitDDO item;
  //     int voxorSize = 0;
  //     bytes->readInt32(voxorSize);

  //     item.voxorId = std::string(voxorSize + 1, '\0');
  //     bytes->reads((Byte *)item.voxorId.data(), voxorSize);

  //     bytes->reads((Byte *)&item.dataId, sizeof(item.dataId));

  //     this->items.push_back(item);
  //   }
  // };

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