#pragma once

#include "DDO.h"
#include "api/UhconnApi.h"
#include "api/UhconnDcoRef.h"
#include "api/UhconnDdo.h"

class DCO {
 private:
  UhconnDcoRef uh_dco;

 public:
  // DCO(){};
  DCO(UhconnDcoRef dco) : uh_dco(dco){};
  DDOId async(std::string function, DDO input);
  DDOId async(std::string function, std::string args);
  DDO wait(DDOId ddoId);

  std::string voxorId();
};