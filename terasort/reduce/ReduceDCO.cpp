#include "ReduceDCO.h"
#include "Step2Task.h"
#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/log.hpp"

std::string ReduceDCO::reduce(std::string a) {
  logger_trace("---------------------------------");
  try {
    PartitionStep2 input;
    input.fromJson(&a);

    Step2Task().run(&input);

    // logger_trace(a.c_str());

    return "success";
  } catch (Exception& ex) {
    logger_error("step2 failed,%s,%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
    return "failed";
  } catch (std::exception& ex) {
    logger_error("step2 failed,%s", ex.what());
    return "failed";
  }
}

ReduceDCO::ReduceDCO() {
  getFunctionTable()["reduce"] = (PTR)&ReduceDCO::reduce;
}
ReduceDCO::~ReduceDCO() {
}
