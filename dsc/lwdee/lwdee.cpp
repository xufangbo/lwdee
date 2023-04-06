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

  int nodeAmount = UhconnConfig::getInstance().getNodeAmount();  
  currentNodeId = currentNodeId % (nodeAmount - 1);

  int nodeId = currentNodeId + 2;

  logger_trace("create dco node%d %s", nodeId, name.c_str());

  UhconnDcoRef uh_dco = UhconnApi::create(nodeId, name);
  DCO dco(uh_dco);

  currentNodeId++;
  return dco;
}

DCO lwdee::create_dco(int nodeId, std::string name) {
  UhconnDcoRef uh_dco = UhconnApi::create(nodeId, name);
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