#include <chrono>
#include <iostream>
#include <string>

#include "Benchmark.h"
#include "UserDco.h"
#include "UserDcoFactory.h"
#include "api/UhconnApi.h"
#include "api/UhconnDcoRef.h"
#include "api/UhconnDdo.h"
#include "api/UhconnDdoRef.h"
#include "core/UhconnRouter.h"
#include "core/UhconnSimpleAddr.h"
#include "core/UhconnVoxorFactory.h"
#include "core/UhconnWorkNode.h"
#include "core/log.hpp"
// #include "edc/driver/Driver.h"
#include "lwdee/lwdee.h"

int test_ddo(void) {
  std::cout << "edc driver" << std::endl;

  auto fileName = "/home/kevin/git/lwdee/edc/harry-potter.txt";
  // Driver().startJob(fileName, 4, 2);

  DDO ddo = lwdee::create_ddo();

  auto bytes = std::make_shared<ByteSpan>(strlen(fileName));
  bytes->puts((Byte*)fileName, bytes->size);
  ddo.write(bytes);

  auto output = ddo.read();

  logger_trace("load blocdata : (%d)%s", output->size(), output->c_str());

  ddo.release();

  return 0;
}

int test_dco(int nodeId) {
  logger_info("< test dco %d", nodeId);

  auto fileName = "/home/kevin/git/lwdee/edc/harry-potter.txt";

  DDO input = lwdee::create_ddo();

  auto bytes = std::make_shared<ByteSpan>(strlen(fileName));
  bytes->puts((Byte*)fileName, bytes->size);
  input.write(bytes);

  DCO dco = lwdee::create_dco(nodeId, "MapDCO");
  auto ddoId = dco.async("f1", input);

  DDO output = dco.wait(ddoId);

  // ByteSpan_ref bytes2 = output.read();
  // std::string str(bytes2->size + 1, '\0');
  // bytes2->reads((Byte*)str.data(), bytes2->size);
  // logger_trace("load blockdata : (%d)%s", str.size(), str.c_str());

  auto str = output.read();
  logger_trace("load blockdata : (%d)%s", str->size(), str->c_str());

  logger_info("> test dco %d", nodeId);

  return 0;
}