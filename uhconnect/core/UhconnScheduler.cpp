#include "UhconnScheduler.h"
#include "UhconnConfig.h"
UhconnScheduler::UhconnScheduler(int nodeID)
{
    NodeId = nodeID;
}

UhconnScheduler::~UhconnScheduler()
{

}

int UhconnScheduler::getDestWorkNode(void) {
    static int count = 0;
    int max_node = UhconnConfig::getInstance().getNodeAmount();
    int ret = (count++ % max_node) + 1;
    #ifdef DEBUGINFO
    std::cout<<"UhconnScheduler to deset node:"<<ret<<std::endl;
    #endif
    return ret;
//    return 80001;
}
