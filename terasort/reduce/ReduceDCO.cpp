#include "ReduceDCO.h"
#include "Step2Task.h"
#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/log.hpp"
#include "matrix/LinuxMatrix.h"
#include "core/Stopwatch.h"

std::string ReduceDCO::reduce(std::string a) {
  logger_trace("---------------------------------");
  try {
    logger_info("< accept reduce ");
    Stopwatch sw;
    // LinuxMatrix::print();
    PartitionStep2 input;
    input.fromJson(&a);

    Step2Task().run(&input);

    // logger_trace(a.c_str());
    // LinuxMatrix::print();
    logger_info("> accept reduct ,partition : %d,eclipse %lf", input.index, sw.stop());

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
