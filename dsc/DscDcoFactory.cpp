#include "DscDcoFactory.h"
#include "core/log.hpp"
#include "map/MapDCO.h"
#include "reduce/ReduceDCO.h"
#include "sample/SampleDCO.h"

DscDcoFactory::DscDcoFactory() {
}

DscDcoFactory::~DscDcoFactory() {
}

UhconnDco* DscDcoFactory::newDco(std::string& class_name) {
  logger_debug("create dco %s", class_name.c_str());
  if (class_name == "SampleDCO") {
    return new SampleDCO();
  } else if (class_name == "MapDCO") {
    return new MapDCO();
  } else if (class_name == "ReduceDCO") {
    return new ReduceDCO();
  }
  return 0;
}