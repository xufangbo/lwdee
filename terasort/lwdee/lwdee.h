#pragma once

#include "DCO.h"

class lwdee {
  private:
  static int currentNodeId;
  public:
   static DCO create_dco(std::string name);
   static DCO create_dco(int nodeId,std::string name);
   static DDO create_ddo();
};