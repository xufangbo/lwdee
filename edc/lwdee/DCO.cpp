#include "DCO.h"

void DCO::async(DDO input) {
  //   UhconnDdo* input
  auto uh_ddo = new UhconnDdo(input.ddoId);
  ddo_ref = UhconnApi::async(uh_dco, function,uh_ddo);
}
DDO DCO::wait() {
  UhconnDdo* ddo = UhconnApi::wait(ddo_ref);
  return DDO(ddo_ref);
}