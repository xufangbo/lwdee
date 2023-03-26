#include "TerasortDCOFactory.h"
#include "map/MapDCO.h"
#include "reduce/ReduceDCO.h"
#include "core/log.hpp"

TerasortDCOFactory::TerasortDCOFactory()
{

}

TerasortDCOFactory::~TerasortDCOFactory()
{

}

UhconnDco* TerasortDCOFactory::newDco(std::string& class_name) {
  logger_debug("create dco %s",class_name.c_str());
  if (class_name == "MapDCO") {
    return new MapDCO();
  } else if (class_name == "ReduceDCO") {
    return new ReduceDCO();
  }
    return 0;
}