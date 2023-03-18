#ifndef RUN_DEMO 

#include "uhshell.h"
#include <iostream>
#include "DemoConfig.h"
#include "core/UhconnVoxorFactory.h"
#include "UserDcoFactory.h"
#include "core/UhconnConfig.h"
#include "core/UhconnWorkNode.h"

extern int kmeans_run(int argc, char *argv[]);

void showVer(void) {
    std::cout << " Version " 
        << Demo_VERSION_MAJOR << "."
        << Demo_VERSION_MINOR << "."
        << Demo_VERSION_PATCH << std::endl;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), 
showVer, showVer, show the version);

int main(int argc, char *argv[]) {

    // 初始化 WorkNode工作环境
    std::string ConfigFile = "test/node_conf.json";
    std::string NodeName = "node1";
    int nd_amt = 2;
    if( argc >= 4 ) {
        ConfigFile = argv[1];
        NodeName = argv[2]; 
        nd_amt = atoi(argv[3]); 
    }
    else {
        std::cout << "usage:"
            << "Demo <cfgfile> <ndname> <ndamount>" 
            << std::endl;
    }
    if(UhconnConfig::getInstance().loadConf(ConfigFile,NodeName) == 0){
        UhconnConfig::getInstance().setNodeAmount(nd_amt);
        UhconnVoxorFactory::getInstance().setupLocalWorkEnvironment(new UserDcoFactory(),UhconnConfig::getInstance().getNodeId()); 
        UhconnWorkNode* workNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
        workNode->itsRouter().setupRouteInfoFromConf();
    }
    else {
        std::cout<<"load config failed!!"<<endl;
    }

    shell_run();
}

#endif