#include "Step1Task.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#include "core/Stopwatch.h"
#include "core/log.hpp"
#include "lwdee/lwdee.h"
#include "map/MapDCO.h"

Step1Output Step1Task::run(PartitionStep1* partition) {
  logger_info("< map task run");
  Stopwatch sw;

  this->partition = partition;

  // textFile
  auto trs = this->textFile();

  // write ddo
  this->generateSubSplit(trs);
  delete trs;

  logger_info("> map task run,eclipse %lf", sw.stop());

  return output;
};

TeraRecords* Step1Task::textFile() {
  logger_info("< textFile");
  Stopwatch sw;

  FILE* f = fopen(partition->fileName.c_str(), "rb");
  if (f == NULL) {
    logger_error("can't open file : %s", partition->fileName.c_str());
  }

  fseek(f, 0, SEEK_END);
  long len = ftell(f) / 100;
  long splitCount = len / partition->sampleSplits.size();

  long start = splitCount * partition->index;
  long end = (start + splitCount) > len ? len : start + splitCount;

  logger_debug("%d - read range, %ld - %ld,计 %ld 条", partition->index, start, end - 1, end - start);

  auto trs = new TeraRecords(end - start, TeraRecord());

  fseek(f, start * 100, SEEK_SET);
  TeraRecord* tr = trs->data();
  for (int i = start; i < end; i++) {
    fread(tr->key, 1, 10, f);
    fread(tr->value, 1, 90, f);

    // logger_trace("%s %s", tr->index().c_str(), tr->line().c_str());

    tr++;
  }

  fclose(f);

  logger_info("> textFile,eclipse %lf", sw.stop());

  return trs;
}

void Step1Task::generateSubSplit(TeraRecords* trs) {
  logger_info("< generateSubSplit");
  Stopwatch sw;

  auto sampleSplits = partition->sampleSplits;

  // 先计算长度
  int counters[sampleSplits.size()];
  memset(&counters, 0, sampleSplits.size() * 4);
  for (TeraRecord& tr : *trs) {
    int splitIndex = classify(tr);
    counters[splitIndex]++;
  }

  // 创建子分区的ByteSpan
  ByteSpan_ref subPartitions[sampleSplits.size()];
  for (int i = 0; i < sampleSplits.size(); i++) {
    logger_debug("partition %d - sub split %d - %d", partition->index, i, counters[i]);
    subPartitions[i] = std::make_shared<ByteSpan>(counters[i] * 100);
  }

  // 遍历TeraRecord，并添加到对应的子分区中去
  for (TeraRecord& tr : *trs) {
    int splitIndex = classify(tr);
    auto bytes = subPartitions[splitIndex];

    bytes->puts(tr.key, 10);
    bytes->puts(tr.value, 90);
  }

  // 保存子分区到DDO
  for (int i = 0; i < sampleSplits.size(); i++) {
    DDO ddoSubSplit = lwdee::create_ddo();
    ddoSubSplit.write(subPartitions[i]);

    MapDCO::ddos.push_back(ddoSubSplit);

    SubSplitDDO item;
    item.voxorId = ddoSubSplit.ddoId.itsVoxorId();
    item.dataId = ddoSubSplit.ddoId.itsId();
    output.items.push_back(item);
  }

  logger_info("> generateSubSplit,eclipse %lf", sw.stop());
}

int Step1Task::classify(TeraRecord& tr) {
  int size = partition->sampleSplits.size();
  uint64_t value = tr.left8();

  for (int i = 0; i < size; i++) {
    SampleSplit& split = partition->sampleSplits[i];
    if (split.min <= 0 && value < split.max) {
      return i;
    } else if (value >= split.min && split.max <= 0) {
      return i;
    } else if (value >= split.min && value < split.max) {
      return i;
    }
  }
  logger_debug("un hint index: %ld", value);
  return -1;
}