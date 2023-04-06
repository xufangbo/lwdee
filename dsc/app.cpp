#include <iostream>

// #include "DemoConfig.h"
// #include "UserDcoFactory.h"
#include <stdlib.h>
#include "DscDcoFactory.h"
#include "core/DscConfig.hpp"
#include "core/UhconnConfig.h"
#include "core/UhconnVoxorFactory.h"
#include "core/UhconnWorkNode.h"
#include "core/log.hpp"
#include "driver/Driver.h"
#include "lwdee/lwdee.h"
#include "matrix/LinuxMatrix.h"

void logger_init(std::string nodeName);
void init(int argc, char* argv[]);
std::string configFile();

int main(int argc, char* argv[]) {
  init(argc, argv);

  auto localNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
  if (localNode == nullptr) {
    logger_error("localNode is null");
    exit(1);
  }
  if (localNode->itId() == 1) {
    Driver().startJob();
  }

  while (true) {
    usleep(1000000);
  }
}

void init(int argc, char* argv[]) {
  std::string nodeName = "node1";
  if (argc >= 2) {
    nodeName = argv[1];
    if (nodeName == "${nodename}") {
      nodeName = getenv("nodename");
      
    }
  } else {
    std::cout << "usage: app demo <ndname>" << std::endl;
  }

  DscConfig::instance()->readConfig();
  logger_init(nodeName);
  logger_warn("this is %s", nodeName.c_str());
  LinuxMatrix::start();

  if (UhconnConfig::getInstance().loadConf(configFile(), nodeName) == 0) {
    // int nodeAmount = UhconnConfig::getInstance().getInt("node_amount");
    // UhconnConfig::getInstance().setNodeAmount(nodeAmount);
    UhconnVoxorFactory::getInstance().setupLocalWorkEnvironment(new DscDcoFactory(), UhconnConfig::getInstance().getNodeId());
    UhconnWorkNode* workNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
    workNode->itsRouter().setupRouteInfoFromConf();
  } else {
    logger_error("load config failed!!");
  }
}

void logger_init(std::string nodeName) {
  LogOption option;
  option.initalized = false;
  option.level = log_trace;
  strcpy(option.path, "./log");
  strcpy(option.name, nodeName.c_str());
  option.is_color = true;
  option.days = 10;

  if (logger_initialize(option) != 0) {
    printf("log initialize error\n");
  } else {
    logger_info("-- app starting ... ");
  }
}

std::string configFile() {
  std::string configFile = "./conf.json";
  FILE* fp = fopen(configFile.c_str(), "r");
  if (fp != NULL) {
    fclose(fp);
  } else {
    configFile = "./config/conf.json";
    fp = fopen(configFile.c_str(), "r");
    if (fp != NULL) {
      fclose(fp);
    } else {
      configFile = "/home/kevin/git/lwdee/config/conf.json";
    }
  }

  return configFile;
}