#include <iostream>

// #include "DemoConfig.h"
// #include "UserDcoFactory.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "core/DscConfig.hpp"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "driver/Driver.h"
#include "kafka/KafkaDCO.h"
#include "map/MapDCO.h"
#include "core/NodeConfig.hpp"
#include "matrix/LinuxMatrix.h"
#include "reduce/ReduceDCO.h"

void logger_init(std::string nodeName);
void init(int argc, char* argv[]);
std::string configFile();
void regist_services();

int main(int argc, char* argv[]) {

  init(argc, argv);
  regist_services();

  auto nodeName = getenv("nodename");
  auto localNode = NodeConfig::byName(nodeName);
  if (localNode->nodeId == 1) {
    Driver().startJob();
  }

  while (true) {
    sleep(1);
  }

  // jsonTest();
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

  NodeConfig::readConfig();

  auto conf = DscConfig::instance();
  conf->readConfig();
  conf->nodeName = nodeName;

  if (conf->name == "local") {
    logger_init(nodeName);
  } else {
    logger_init("dsc");
  }

  logger_warn("this is %s", nodeName.c_str());
}

void logger_init(std::string nodeName) {
  LogOption option;
  option.initalized = false;
  option.level = log_trace;
  strcpy(option.fileMode, "w+");
  strcpy(option.path, "./log");
  strcpy(option.name, nodeName.c_str());
  option.is_color = true;
  option.days = 10;
  option.writeFile = true;

  if (logger_initialize(option) != 0) {
    printf("log initialize error\n");
  } else {
    logger_info("-- app starting ... ");
  }
}

void regist_services() {
  regist_kafka_start_service();
  regist_map_start_service();
  regist_map_invoke_service();
  regist_reduce_start_service();
  regist_reduce_invoke_service();
}