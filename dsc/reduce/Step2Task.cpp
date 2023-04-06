#include "Step2Task.h"

#include <map>

#include "core/Exception.hpp"
#include "core/Stopwatch.h"
#include "core/log.hpp"
#include "lwdee/lwdee.h"

Step2Output Step2Task::run(PartitionStep2* partition) {
  this->input = partition;

  logger_info("< reduce task run");
  Stopwatch sw;

  // std::vector<TeraRecord*>* trs = new std::vector<TeraRecord*>();

  this->read();

  this->save();

  logger_info("> reduce task run,partition: %d,output count: %d,eclipse %lf, %s", input->index, size, sw.stop(), this->fileName().c_str());

  return this->output;
}

void Step2Task::read() {
  logger_info("< read");
  Stopwatch sw;

  string_ref subsplits[input->subSplitDDOs.size()];

  unsigned long totalSize = 0;
  for (int i = 0; i < input->subSplitDDOs.size(); i++) {
    SubSplitDDO& x = input->subSplitDDOs[i];
    DDO ddo(x.voxorId, x.dataId);

    try {
      auto str = ddo.read();

      subsplits[i] = str;
      totalSize += str->size();

      logger_debug("read node %d ddo, record count: %ld条", i + 1, str->size() / 100);

      ddo.releaseGlobal();
    } catch (Exception& ex) {
      ex.trace(ZONE);
    }
  }

  this->size = totalSize / 100;
  logger_debug("read 所有分区DDO记录 合计%ld条", size);

  logger_debug("开始生成TeraRecord记录，应该是%ld条", size);

  int ddoIndex = 0;
  for (auto subsplit : subsplits) {
    logger_debug("%d - split %d,%ld条", input->index, ddoIndex, subsplit->size() / 100);
    ddoIndex++;

    char* ptr = (char*)subsplit->data();
    for (int i = 0; i < subsplit->size(); i += 100) {
      // TeraRecord& tr = trs[tri];
      // memcpy(tr.key, ptr, 10);
      // ptr += 10;
      // memcpy(tr.value, ptr, 90);
      // ptr += 90;
      // tri++;

      // logger_trace("%s %s", tr.index().c_str(), tr.line().c_str());
    }

    subsplit.reset();
  }
  logger_info("> read,partition: %d,eclipse %lf", input->index, sw.stop());
}

void Step2Task::save() {
  logger_info("< save");
  Stopwatch sw;

 

  logger_info("> save,partition: %d,eclipse %lf", input->index, sw.stop());
}