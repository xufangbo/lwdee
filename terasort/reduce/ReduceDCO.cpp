#include "ReduceDCO.h"
#include "Step2Task.h"
#include "core/Partition.h"
#include "core/log.hpp"

std::string ReduceDCO::reduce(std::string a) {
  PartitionStep2 input;
  input.fromJson(&a);

  Step2Task().run(&input);

  logger_trace(a.c_str());

  return "success";
}

ReduceDCO::ReduceDCO() {
  getFunctionTable()["reduce"] = (PTR)&ReduceDCO::reduce;
}
ReduceDCO::~ReduceDCO() {
}
