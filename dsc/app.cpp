#include <iostream>

// #include "DemoConfig.h"
// #include "UserDcoFactory.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client/SocketScheduler.hpp"
#include "core/DscConfig.hpp"
#include "core/NodeConfig.hpp"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "driver/Driver.h"
#include "kafka/KafkaDCO.h"
#include "map/MapSchedule.hpp"
#include "matrix/LinuxMatrix.h"
#include "reduce/ReduceDCO.h"
#include "server/Leopard.hpp"

void logger_init(std::string nodeName);
void init(int argc, char* argv[]);
std::string configFile();
void regist_services();

int main(int argc, char* argv[]) {
  init(argc, argv);

  TNode* node = NodeConfig::local;

  regist_services();
  SocketScheduler::start();

  if (node->nodeId == 1) {
    Driver().startJob();
    while (true) {
      sleep(10);
    }
  } else {
    Leopard leopard(0);
    leopard.start("0.0.0.0", node->port);
    // leopard.start(node->ip.c_str(), node->port);
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

  NodeConfig::readConfig();
  NodeConfig::setLocal(nodeName);

  auto conf = DscConfig::instance();
  conf->readConfig();
  conf->nodeName = nodeName;

  if (conf->name == "local") {
    read_log_config(nodeName.c_str());
  } else {
    read_log_config("dsc");
  }

  logger_warn("this is %s", nodeName.c_str());
}

void regist_services() {
  regist_kafka_start_service();
  regist_map_start_service();
  regist_map_invoke_service();
  regist_reduce_start_service();
  regist_reduce_invoke_service();
}