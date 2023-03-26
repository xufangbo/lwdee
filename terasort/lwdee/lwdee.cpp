#include "lwdee.h"

#include "api/UhconnApi.h"
#include "api/UhconnDcoRef.h"
#include "api/UhconnDdoRef.h"
#include "core/UhconnVoxorFactory.h"
#include "core/UhconnWorkNode.h"

DCO lwdee::create_dco(std::string name) {
  UhconnDcoRef uh_dco = UhconnApi::create(name);
  DCO dco(uh_dco);
  return dco;
}

DCO lwdee::create_dco(int nodeId,std::string name) {
  UhconnDcoRef uh_dco = UhconnApi::create(nodeId,name);
  DCO dco(uh_dco);
  return dco;
}

DDO lwdee::create_ddo() {
  auto addr = UhconnVoxorFactory::getInstance()
                  .getLocalWorkNode()
                  ->itsDeamon()
                  .itsAddr();

  return DDO(UhconnDdoRef(addr));
}