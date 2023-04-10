#include "ReduceDCO.h"
#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/Stopwatch.h"
#include "core/log.hpp"
#include "matrix/LinuxMatrix.h"

std::string ReduceDCO::reduce(std::string a) {
  try {
    logger_info("reduce dco accept");
    // logger_info("reduce dco accept %s", a.c_str());

    /**
     * reduce要集齐所有的map再计算才有意义
     * 这里的json也会崩溃
     */



    // Stopwatch sw;
    // LinuxMatrix::print();
    // PartitionReduce input;
    // input.fromJson(&a);

    // Step2Task().run(&input);

    // // logger_trace(a.c_str());
    // LinuxMatrix::print();

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
