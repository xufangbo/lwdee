#include "lwdee.h"

#include "api/UhconnDdoRef.h"
#include "core/UhconnVoxorFactory.h"
#include "core/UhconnWorkNode.h"

DCO lwdee::createDCO(std::string name) {
  DCO dco(name);
  return dco;
}

DDO lwdee::createDDO() {

  auto addr = UhconnVoxorFactory::getInstance()
                  .getLocalWorkNode()
                  ->itsDeamon()
                  .itsAddr();
                  
  return DDO(UhconnDdoRef(addr));
}