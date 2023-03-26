#include "MapDCO.h"

#include <iostream>

#include "Step1Task.h"
#include "core/Partition.h"
#include "core/log.hpp"

std::vector<DDO> MapDCO::ddos;

std::string MapDCO::f1(std::string a) {
  // logger_debug("invokded f1 , args : %s", a.c_str());

  DDO ddo(a);
  logger_debug("invokded f1 , ddo voxor : %s,ddo id: %ld", ddo.ddoId.itsVoxorId().c_str(), ddo.ddoId.itsId());
  return a;
}

std::string MapDCO::map(std::string a) {
  logger_debug("< invokded map %s",a.c_str());

  PartitionStep1 input;
  input.fromJson(a);

  // logger_debug("invokded map ,index : %d, fileName: %s,subsplit count %d", input.index, input.fileName.c_str(),input.sampleSplits.size());
  // for(SampleSplit &split : input.sampleSplits){
  //   logger_debug("{min:%ld,max:%ld}", split.min, split.max);
  // }

  Step1Task().run(&input);

  logger_debug("> invokded map ,index : %d, fileName: %s", input.index, input.fileName.c_str());

  return std::to_string(input.outputDDO.ddoId.itsId());
}

std::string MapDCO::ddo(std::string voxorId, DdoDataId ddoId) {
  std::cout << "call f2(" << voxorId << ")" << std::endl;
  return voxorId;
}
MapDCO::MapDCO() {
  getFunctionTable()["f1"] = (PTR)&MapDCO::f1;
  getFunctionTable()["map"] = (PTR)&MapDCO::map;
  getFunctionTable()["ddo"] = (PTR)&MapDCO::ddo;
}
MapDCO::~MapDCO() {}
