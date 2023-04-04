#include "DCO.h"

DDOId DCO::async(std::string function, DDO input) {
  auto uh_ddo = new UhconnDdo(input.ddoId);   
  DDOId ddo_ref = UhconnApi::async(uh_dco, function, uh_ddo);
  return ddo_ref;
}

DDOId DCO::async(std::string function, std::string args) {
  DDOId ddo_ref = UhconnApi::async(uh_dco, function, args);
  return ddo_ref;
}

DDO DCO::wait(DDOId ddoId) {
  UhconnDdo* ddo = UhconnApi::wait(ddoId); // 会有内存泄漏
  return DDO(ddo->itsRef());
}