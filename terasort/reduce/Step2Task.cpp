#include "Step2Task.h"

#include <map>

#include "core/Exception.hpp"
#include "core/TeraRecord.hpp"
#include "core/log.hpp"
#include "lwdee/lwdee.h"

Step2Output Step2Task::run(PartitionStep2* partition) {
  this->input = partition;

  logger_info("< run");

  // std::vector<TeraRecord*>* trs = new std::vector<TeraRecord*>();

  this->read();
  logger_info("read");

  this->sort();
  logger_info("sort");

  this->save();
  logger_info("save");

  logger_info("> run,output count: %d, %s", size, this->fileName().c_str());

  return this->output;
}

void Step2Task::read() {
  string_ref subsplits[input->subSplitDDOs.size()];

  unsigned long totalSize = 0;
  for (int i = 0; i < input->subSplitDDOs.size(); i++) {
    SubSplitDDO& x = input->subSplitDDOs[i];
    DDO ddo(x.voxorId, x.dataId);
    auto str = ddo.read();

    subsplits[i] = str;
    totalSize += str->size();

    logger_debug("read node %d ddo, record count: %ld条", i + 1, str->size() / 100);

    ddo.releaseGlobal();
  }

  this->size = totalSize / 100;
  logger_debug("read 所有分区DDO记录 合计%ld条", size);

  logger_debug("开始生成TeraRecord记录，应该是%ld条", size);
  trs = new TeraRecord[size];
  int tri = 0;

  int ddoIndex = 0;
  for (auto subsplit : subsplits) {
    logger_debug("%d - split %d,%ld条", input->index, ddoIndex, subsplit->size() / 100);
    ddoIndex++;

    char* ptr = (char*)subsplit->data();
    for (int i = 0; i < subsplit->size(); i += 100) {
      TeraRecord& tr = trs[tri];
      memcpy(tr.key, ptr, 10);
      ptr += 10;
      memcpy(tr.value, ptr, 90);
      ptr += 90;
      tri++;

      // logger_trace("%s %s", tr.index().c_str(), tr.line().c_str());
    }

    subsplit.reset();
  }
}

int teraCompare(const void* a, const void* b) {
  return memcmp(((TeraRecord*)a)->key, ((TeraRecord*)b)->key, 10);
  // TeraRecord* left = (TeraRecord*)a;
  // TeraRecord* right = (TeraRecord*)b;

  // auto l = left->left8();
  // auto r = right->left8();

  // if (l > r) {
  //   return 1;
  // } else {
  //   return l < r ? -1 : 0;
  // }
}

void Step2Task::sort() {
  qsort(trs, size, sizeof(TeraRecord), teraCompare);
}

void Step2Task::save() {
  FILE* f = fopen(fileName().c_str(), "w");
  if (f == NULL) {
    throw Exception("can't open file " + input->outputFile, ZONE);
  }

  for (int i = 0; i < size; i++) {
    TeraRecord& tr = trs[i];

    // auto id = std::to_string(tr.left8()) + " ";
    // fwrite(id.c_str(), id.size(), 1, f);

    fwrite(tr.key, 10, 1, f);
    fwrite(tr.value, 90, 1, f);
  }

  fclose(f);
}