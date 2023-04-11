#include "lwdee.h"

#include "api/UhconnApi.h"
#include "api/UhconnDcoRef.h"
#include "api/UhconnDdoRef.h"
#include "core/UhconnConfig.h"
#include "core/UhconnVoxorFactory.h"
#include "core/UhconnWorkNode.h"
#include "core/log.hpp"

int lwdee::currentNodeId = 0;

DCO lwdee::create_dco(std::string name) {
  DCO dco = create_dco_byindex(currentNodeId, name);
  currentNodeId++;
  return dco;
}

DCO lwdee::create_dco_byindex(int indexId, std::string name) {
  int nodeAmount = UhconnConfig::getInstance().getNodeAmount();
  indexId = indexId % (nodeAmount - 1);
  int nodeId = indexId + 2;

  UhconnDcoRef uh_dco = UhconnApi::create(name, nodeId);
  DCO dco(uh_dco);

  // logger_trace("get create dco voxorId:%s", uh_dco.itsBelongVoxorId().c_str());

  return dco;
}

DCO lwdee::create_dco(int nodeId, std::string name) {
  UhconnDcoRef uh_dco = UhconnApi::create(name, nodeId);
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