#include "Step2Task.h"

#include <map>

#include "core/MapSerializer.h"
#include "core/TuplesSerialzer.h"
#include "lwdee/lwdee.h"

DDO Step2Task::run(PartitionStep2 *partition) {
  map<string, int> mapOutputs;

  for (DDO &ddoInput : partition->ddos) {
    // 读取DDO数据
    auto bytesInput = ddoInput.read();

    Map_ref inputMap = MapSerializer().deserailize(bytesInput.get());
    ddoInput.release();  // 销毁input数据

    // 执行分组
    for (auto t : *inputMap) {
      mapOutputs[t.first] += t.second;
    }
  }

  // 生成结果
  // write ddo
  ByteSpan_ref bytesOutput = MapSerializer().serailize(&mapOutputs);

  DDO ddoOutput = lwdee::createDDO();
  ddoOutput.write(bytesOutput);
  return ddoOutput;
}