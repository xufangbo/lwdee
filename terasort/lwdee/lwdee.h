#pragma once

#include "DCO.h"

class lwdee {
  public:
   static DCO create_dco(std::string name, std::string functionName);
   static DDO create_ddo();
};