#include "MapDCO.h"

#include <iostream>
#include <sstream>

#include "Step1Task.h"
#include "core/Partition.h"
#include "core/log.hpp"

std::vector<DDO> MapDCO::ddos;

std::string MapDCO::f1(std::string a) {
  logger_debug("invokded f1 , args : %s", a.c_str());
  return a;
}

std::string MapDCO::map(std::string a) {
  logger_debug("< invokded map %s", a.c_str());

  PartitionStep1 input;
  input.fromJson(&a);

  auto output = Step1Task().run(&input);

  logger_debug("> invokded map ,index : %d, ddoId: %ld, fileName: %s", input.index, input.outputDDO.ddoId.itsId(), input.fileName.c_str());

  return output.toJson();
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
