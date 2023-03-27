#include "Step2Task.h"

#include <map>

#include "core/MapSerializer.h"
#include "core/TuplesSerialzer.h"
#include "lwdee/lwdee.h"

Step2Output Step2Task::run(PartitionStep2* partition) {
  this->input = partition;

  for (SubSplitDDO& i : partition->subSplitDDOs) {
    DDO ddo(i.voxorId, i.dataId);
    auto str = ddo.read();

    break;
  }

  return this->output;
}