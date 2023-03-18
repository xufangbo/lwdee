#include "lwdee.h"

DCO lwdee::createDCO(std::string name) {
  DCO dco(name);
  return dco;
}

DDO lwdee::createDDO() {
  DDO ddo;
  ddo.id = DdoManager::generateId(); 
  return ddo;
}