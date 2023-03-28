#include <iostream>

#include "DemoConfig.h"
#include "UserDcoFactory.h"
#include "core/UhconnConfig.h"
#include "core/UhconnVoxorFactory.h"
#include "core/UhconnWorkNode.h"
#include "core/log.hpp"
#include "driver/Driver.h"
#include "lwdee/lwdee.h"
#include "terasort.h"
#include "terasort/TerasortDCOFactory.h"
#include "uhshell.h"

void init_logger();
void init(int argc, char* argv[]);

int main(int argc, char* argv[]) {
  init(argc, argv);

  auto localNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
  if (localNode == nullptr) {
    logger_error("localNode is null");
    exit(1);
  }
  if (localNode->itId() == 1) {
    
    auto inputFile = UhconnConfig::getInstance().getInputFile();
    auto outputFile = UhconnConfig::getInstance().getOutputFile();

    logger_debug("%s %s", inputFile.c_str(), outputFile.c_str());

    int nodeAmount = UhconnConfig::getInstance().getNodeAmount();
    Driver().startJob(inputFile, outputFile, 3, nodeAmount, nodeAmount);

    // test_dco(1);
    // test_dco(2);
    // test_dco(3);
  }

  while (true) {
    usleep(1000000);
  }
}

void init(int argc, char* argv[]) {
  init_logger();

  std::string nodeName = "node1";
  if (argc >= 2) {
    nodeName = argv[1];
  } else {
    std::cout << "usage: app demo <ndname>" << std::endl;
  }

  std::string configFile = "./node_conf.json";
  FILE* fp = fopen(configFile.c_str(), "r");
  if (fp != NULL) {
    fclose(fp);
  } else {
    configFile = "../test/node_conf.json";
    fp = fopen(configFile.c_str(), "r");
    if (fp != NULL) {
      fclose(fp);
    } else {
      configFile = "/home/kevin/git/lwdee/test/node_conf.json";
    }
  }

  if (UhconnConfig::getInstance().loadConf(configFile, nodeName) == 0) {
    // int nodeAmount = UhconnConfig::getInstance().getInt("node_amount");
    // UhconnConfig::getInstance().setNodeAmount(nodeAmount);
    UhconnVoxorFactory::getInstance().setupLocalWorkEnvironment(new TerasortDCOFactory(), UhconnConfig::getInstance().getNodeId());
    UhconnWorkNode* workNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
    workNode->itsRouter().setupRouteInfoFromConf();
  } else {
    logger_error("load config failed!!");
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