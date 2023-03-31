#include "SampleDCO.h"

#include <iostream>
#include <sstream>

#include "Step0Task.h"
#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/log.hpp"
#include "matrix/LinuxMatrix.h"

std::vector<DDO> SampleDCO::ddos;

std::string SampleDCO::sample(std::string a) {
  try {
    logger_info("< accept sample ");
    LinuxMatrix::print();
    // logger_info("< invokded map %s", a.c_str());

    PartitionStep0 input;
    input.fromJson(&a);

    MinAndMax output = Step0Task().run(&input);

    LinuxMatrix::print();
    logger_info("> accept sample ,partition : %d, ddoId: %ld, fileName: %s", input.index, input.outputDDO.ddoId.itsId(), input.fileName.c_str());

    return std::to_string(output.first.left8()) + "-" + std::to_string(output.second.left8());

  } catch (Exception& ex) {
    logger_error("step2 failed,%s,%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
    return "failed";
  } catch (std::exception& ex) {
    logger_error("step2 failed,%s", ex.what());
    return "failed";
  }
}

SampleDCO::SampleDCO() {
  getFunctionTable()["sample"] = (PTR)&SampleDCO::sample;
}
SampleDCO::~SampleDCO() {}
