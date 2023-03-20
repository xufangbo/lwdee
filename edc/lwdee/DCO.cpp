#include "DCO.h"

void DCO::async(DDO input) {
  //   UhconnDdo* input
  ddo_ref = UhconnApi::async(uh_dco, function, input.uh_ddo.get());
}
DDO DCO::wait() {
  UhconnDdo* ddo = UhconnApi::wait(ddo_ref);
  return DDO(ddo_ref,ddo);
}