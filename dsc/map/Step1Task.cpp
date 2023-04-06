#include "Step1Task.h"

#include "core/Stopwatch.h"
#include "core/log.hpp"
#include "lwdee/lwdee.h"
#include "map/MapDCO.h"


std::string Step1Task::start(PartitionStep1* partition) {
  logger_info("< map task start");
  Stopwatch sw;

  this->partition = partition;

  
  

  logger_info("> map task start,eclipse %lf", sw.stop());

  return "success";
};