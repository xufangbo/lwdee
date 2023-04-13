#include "MapDCO.h"

#include <iostream>
#include <sstream>

#include "Step1Task.h"
#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/Stopwatch.h"
#include "core/log.hpp"
#include "matrix/LinuxMatrix.h"

std::vector<DDO> MapDCO::ddos;

std::string MapDCO::f1(std::string a) {
  logger_debug("invokded f1 , args : %s", a.c_str());
  return a;
}

std::string MapDCO::map(std::string a) {
  try {
    logger_info("< accept map ");
    Stopwatch sw;
    // LinuxMatrix::print();
    // logger_info("< invokded map %s", a.c_str());

    PartitionStep1 input;
    input.fromJson(&a);

    auto output = Step1Task().run(&input);

    // LinuxMatrix::print();
    logger_info("> accept map ,partition : %d, ddoId: %ld, fileName: %s,eclipse %lf", input.index, input.outputDDO.ddoId.itsId(), input.fileName.c_str(), sw.stop());

    return output.toJson();
  } catch (Exception& ex) {
    logger_error("step2 failed,%s,%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
    return "failed";
  } catch (std::exception& ex) {
    logger_error("step2 failed,%s", ex.what());
    return "failed";
  }
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
