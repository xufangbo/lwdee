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
#include "driver/Driver.h"
#include "lwdee/lwdee.h"

int simple_main(void) {
  std::cout << "simple test start!" << std::endl;
  // 创建用户DCO
  UhconnDcoRef dcoA = UhconnApi::create(std::string("UserDco"));

  UhconnDdoRef df(UhconnVoxorFactory::getInstance().getLocalWorkNode()->itsDeamon().itsAddr());
  UhconnDdo para(df);

  DdoBlockData test_data;
  test_data.type = 22;
  test_data.len = 10;
  test_data.data = new char[10];
  memcpy(test_data.data, "test data", 10);
  para.storeBlock(test_data);
  delete[] (char*)test_data.data;
  
  UhconnDdoRef ddoref = UhconnApi::async(dcoA, std::string("f1"), &para);

  UhconnDdo* ddo = UhconnApi::wait(ddoref);

  DdoBlockData result;
  ddo->loadBlock(result);
  std::cout << "result len:" << result.len << "type:" << result.type << std::endl;
  std::cout << "simple test completed!" << std::endl;
  return 0;
}

int edc_driver(void) {
  std::cout << "edc driver" << std::endl;

  auto fileName = "/home/kevin/git/lwdee/edc/harry-potter.txt";
  // Driver().startJob(fileName, 4, 2);

  DDO ddo = lwdee::create_ddo();

  auto bytes = std::make_shared<ByteSpan>(strlen(fileName));
  bytes->puts((Byte*)fileName, bytes->size);
  ddo.write(bytes);

  auto bytes2 = ddo.read();
  std::string str(bytes2->size + 1, '\0');
  bytes2->reads((Byte*)str.data(), bytes2->size);

  logger_trace("load blocdata : (%d)%s", str.size(), str.c_str());

  ddo.release();

  return 0;
}

int edctest(void) {
  std::cout << "edc test start!" << std::endl;

  auto fileName = "/home/kevin/git/lwdee/edc/harry-potter.txt";

  DDO input = lwdee::create_ddo();

  auto bytes = std::make_shared<ByteSpan>(strlen(fileName));
  bytes->puts((Byte*)fileName, bytes->size);
  input.write(bytes);

  DCO dco = lwdee::create_dco("UserDco", "f1");
  dco.async(input);

  // UhconnDcoRef dcoA = UhconnApi::create(std::string("UserDco"));
  // UhconnDdoRef ddoref = UhconnApi::async(dcoA, std::string("f1"), input.uh_ddo.get());

  DDO output = dco.wait();
  // UhconnDdo* ddo = UhconnApi::wait(ddoref);
  //  DDO output(ddoref,ddo);

  ByteSpan_ref bytes2 = output.read();
  std::string str(bytes2->size + 1, '\0');
  bytes2->reads((Byte*)str.data(), bytes2->size);

  logger_trace("load blockdata : (%d)%s", str.size(), str.c_str());

  return 0;
}