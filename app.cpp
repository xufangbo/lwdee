#include "uhshell.h"
#include <iostream>
#include "DemoConfig.h"
#include "core/UhconnVoxorFactory.h"
#include "UserDcoFactory.h"
#include "core/UhconnConfig.h"
#include "core/UhconnWorkNode.h"

#include "core/log.hpp"
#include "edc/driver/Driver.h"
#include "lwdee/lwdee.h"
#include "edc.h"

void init_logger();
void init(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  init(argc, argv);

  // edc_driver();
  if (UhconnVoxorFactory::getInstance().getLocalWorkNode()->itId() == 2) {
    logger_debug("this is node2");
    simple_main();
  }

  while(true){
      usleep(1000000);
  }
}

void init(int argc, char *argv[]) {

    init_logger();

    // 初始化 WorkNode工作环境
    std::string configFile = "../test/node_conf.json";
    std::string nodeName = "node1";
    int nd_amt = 2;
    if( argc >= 2 ) {
        nodeName = argv[1];
    }
    else {
        std::cout << "usage:"  << "app demo <ndname>"  << std::endl;
    }
    if(UhconnConfig::getInstance().loadConf(configFile,nodeName) == 0){
        UhconnConfig::getInstance().setNodeAmount(nd_amt);
        UhconnVoxorFactory::getInstance().setupLocalWorkEnvironment(new UserDcoFactory(),UhconnConfig::getInstance().getNodeId()); 
        UhconnWorkNode* workNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
        workNode->itsRouter().setupRouteInfoFromConf();
    }
    else {
        std::cout<<"load config failed!!"<<endl;
    }
}

void init_logger() {

  // LogOption option{false, log_trace, "../log", "edc", true, 10};
  LogOption option;
  option.initalized = false;
  option.level = log_trace;
  strcpy(option.path,"../log");
  strcpy(option.name,"../edc");
  option.is_color = true;
  option.days = 10;

  if (logger_initialize(option) != 0) {
    printf("log initialize error\n");
  } else {
    logger_info("-- app starting ... ");
  }
}