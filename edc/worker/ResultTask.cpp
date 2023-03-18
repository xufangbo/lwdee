#include "ResultTask.h"

#include <map>

#include "core/TuplesSerialzer.h"
#include "core/MapSerializer.h"
#include "lwdee/lwdee.h"

DDO ResultTask::runTask(Partition *p) {

  PartitionStage *partition = static_cast<PartitionStage *>(p);

  // 读取DDO数据
  DDO ddoInput = partition->ddo;
  auto bytesInput = ddoInput.read();

  TuplesSerialzer tuplesSerializer;
  Tuples_ref tuples = tuplesSerializer.deserailize(bytesInput.get());
  ddoInput.release(); // 销毁input数据

  // 执行分组
  map<string, int> groups;
  for (auto t : *tuples) {
    groups[get<0>(t)]++;
  }

  // 生成结果
  // write ddo
  MapSerializer mapSerializer;
  ByteSpan_ref bytesOutput = mapSerializer.serailize(&groups);

  DDO ddoOutput = lwdee::createDDO();
  ddoOutput.write(bytesOutput);
  return ddoOutput;
}