#pragma once

#include "DDO.h"
#include "api/UhconnApi.h"
#include "api/UhconnDcoRef.h"
#include "api/UhconnDdo.h"

class DCO {
 private:
  std::string function;
  UhconnDcoRef uh_dco;
  UhconnDdoRef ddo_ref;

 public:
  DCO(UhconnDcoRef dco, std::string function)
      : uh_dco(dco), function(function){};
  void async(DDO input);
  DDO wait();
};