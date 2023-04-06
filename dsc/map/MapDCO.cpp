#include "MapDCO.h"

#include <iostream>
#include <sstream>

#include "KafkaJobConsumer.hpp"
#include "Step1Task.h"
#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/Stopwatch.h"
#include "core/log.hpp"
#include "matrix/LinuxMatrix.h"

std::vector<DDO> MapDCO::ddos;

std::string MapDCO::start(std::string a) {
  try {
    logger_info("< accept start ");
    Stopwatch sw;
    LinuxMatrix::print();
    // logger_info("< invokded start %s", a.c_str());

    PartitionStep1 input;
    input.fromJson(&a);

    KafkaJobConsumer::start(input.index);

    LinuxMatrix::print();
    logger_info("> accept start ,partition : %d,eclipse %lf", input.index, sw.stop());

    return "succeed";
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
  getFunctionTable()["start"] = (PTR)&MapDCO::start;
  getFunctionTable()["ddo"] = (PTR)&MapDCO::ddo;
}
MapDCO::~MapDCO() {}
