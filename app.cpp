#include <iostream>
#include "DemoConfig.h"
#include "UserDcoFactory.h"
#include "core/UhconnConfig.h"
#include "core/UhconnVoxorFactory.h"
#include "core/UhconnWorkNode.h"
#include "uhshell.h"

#include "core/log.hpp"
#include "terasort.h"
#include "lwdee/lwdee.h"
#include "terasort/TerasortDCOFactory.h"
#include "driver/Driver.h"

void init_logger();
void init(int argc, char* argv[]);

int main(int argc, char* argv[]) {
  init(argc, argv);

  Driver driver;
  driver.startJob("/home/kevin/git/lwdee/terasort/data-input.dat",4,4);

//   auto localNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
//   if (localNode == nullptr) {
//     logger_error("localNode is null");
//     exit(1);
//   }
//   if (localNode->itId() == 2) {
//     for (int i = 0; i < 10; i++) {
//       // simple_main();
//       edctest();
//       edc_driver();
//     }
//   }

//   while (true) {
//     usleep(1000000);
//   }
}

void init(int argc, char* argv[]) {
  init_logger();

  // 初始化 WorkNode工作环境
  std::string configFile = "/home/kevin/git/lwdee/test/node_conf.json";
  std::string nodeName = "node1";
  int nd_amt = 2;
  if (argc >= 2) {
    nodeName = argv[1];
  } else {
    std::cout << "usage: app demo <ndname>" << std::endl;
  }
  if (UhconnConfig::getInstance().loadConf(configFile, nodeName) == 0) {
    UhconnConfig::getInstance().setNodeAmount(nd_amt);
    UhconnVoxorFactory::getInstance().setupLocalWorkEnvironment(new TerasortDCOFactory(), UhconnConfig::getInstance().getNodeId());
    UhconnWorkNode* workNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
    workNode->itsRouter().setupRouteInfoFromConf();
  } else {
    std::cout << "load config failed!!" << endl;
  }
}

void init_logger() {
  // LogOption option{false, log_trace, "../log", "edc", true, 10};
  LogOption option;
  option.initalized = false;
  option.level = log_trace;
  strcpy(option.path, "../log");
  strcpy(option.name, "../edc");
  option.is_color = true;
  option.days = 10;

  if (logger_initialize(option) != 0) {
    printf("log initialize error\n");
  } else {
    logger_info("-- app starting ... ");
  }
}