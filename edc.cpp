/**
 * @file simple_run.cpp
 * @author your name (shiqingpeng@domain.com)
 * @brief  对UhomeConnect组件接口的基本调用演示
 * @version 0.1
 * @date 2022-08-05
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <libgo.h>
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
#include "shell_cpp.h"

#include "core/log.hpp"
#include "edc/driver/Driver.h"
#include "lwdee/lwdee.h"

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
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), edcdriver, edc_driver, run edc driver);

int edctest(void) {

  auto fileName = "/home/kevin/git/lwdee/edc/harry-potter.txt";

  DDO input = lwdee::create_ddo();

  auto bytes = std::make_shared<ByteSpan>(strlen(fileName));
  bytes->puts((Byte*)fileName, bytes->size);
  input.write(bytes);

  DCO dco = lwdee::create_dco("UserDco", "f1");
  dco.async(input);

  DDO output = dco.wait();

  ByteSpan_ref bytes2 = output.read();
  std::string str(bytes2->size + 1, '\0');
  bytes2->reads((Byte*)str.data(), bytes2->size);

  logger_trace("load blocdata : (%d)%s", str.size(), str.c_str());

  return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), edctest, edctest, run edc test);