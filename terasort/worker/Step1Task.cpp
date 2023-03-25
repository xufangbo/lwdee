#include "Step1Task.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#include "core/MapSerializer.h"
#include "core/log.hpp"
#include "lwdee/lwdee.h"

PartitionStep1 Step1Task::run(PartitionStep1 *partition) {
  this->partition = partition;

  // textFile
  auto trs = this->textFile();

  // write ddo
  this->generateSubSplit(trs);
  delete trs;

  return *partition;
};

TeraRecords *Step1Task::textFile() {
  FILE *f = fopen(partition->fileName.c_str(), "rb");
  if (f == NULL) {
    logger_error("can't open file : %s", partition->fileName.c_str());
  }

  fseek(f, 0, SEEK_END);
  long len = ftell(f) / 100;
  long splitCount = len / partition->sampleSplits.size();

  long start = splitCount * partition->index;
  long end = start + splitCount > len ? len : start + splitCount;

  auto trs = new TeraRecords(end - start, TeraRecord());

  fseek(f, start, SEEK_SET);
  TeraRecord *tr = trs->data();
  for (int i = start; i < end; i++) {
    fread(tr->key, 1, 10, f);
    fread(tr->value, 1, 90, f);
    tr++;

    // logger_trace("%d - %ld", partition->index, tr.index());
  }
  fclose(f);

  return trs;
}

void Step1Task::generateSubSplit(TeraRecords *trs) {
  auto sampleSplits = partition->sampleSplits;

  // 先计算长度
  int counters[sampleSplits.size()];
  memset(&counters, 0, sampleSplits.size() * 4);
  for (TeraRecord &tr : *trs) {
    int splitIndex = classify(tr);
    counters[splitIndex]++;
  }

  ByteSpan_ref subPartitions[sampleSplits.size()];
  for (int i = 0; i < sampleSplits.size(); i++) {
    logger_debug("partition %d - sub split %d - %d", partition->index, i,
                 counters[i]);
    subPartitions[i] = std::make_shared<ByteSpan>(counters[i] * 100);
  }

  for (TeraRecord &tr : *trs) {
    int splitIndex = classify(tr);
    auto bytes = subPartitions[splitIndex];

    bytes->puts(tr.key, 10);
    bytes->puts(tr.value, 90);
  }

  vector<DDO> ddos;
  for (int i = 0; i < sampleSplits.size(); i++) {
    DDO ddo = lwdee::create_ddo();
    ddo.write(subPartitions[i]);

    partition->ddos.push_back(ddo);
  }
}

int Step1Task::classify(TeraRecord &tr) {
  int size = partition->sampleSplits.size();
  uint64_t value = tr.left8();

  for (int i = 0; i < size; i++) {
    SampleSplit &split = partition->sampleSplits[i];
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