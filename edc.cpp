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
#include "core/UhconnVoxorFactory.h"
#include "UserDco.h"
#include "api/UhconnApi.h"
#include "api/UhconnDcoRef.h"
#include "core/UhconnRouter.h"
#include "core/UhconnWorkNode.h"
#include <iostream>
#include <string>
#include <libgo.h>
#include "UserDcoFactory.h"
#include "api/UhconnDdo.h"
#include "api/UhconnDcoRef.h"
#include "api/UhconnDdoRef.h"
#include "shell_cpp.h"
#include "core/UhconnSimpleAddr.h"
#include <chrono>
#include "Benchmark.h"

#include "edc/driver/Driver.h"

int edc_driver(void) {

  std::cout << "edc driver" << std::endl;

  auto fileName = "/home/kevin/git/lwdee/edc/harry-potter.txt";
  Driver().startJob(fileName, 4, 2);

  return 0;
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), edcdriver, edc_driver, run edc driver);
