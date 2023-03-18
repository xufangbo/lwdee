#include "UhconnWorkNode.h"
#include "UhconnVoxor.h"

#include <iostream>
#include <libgo.h>
#include <string>

using namespace std;
UhconnWorkNode::UhconnWorkNode(int ad) : scheduler(ad), router(this), _db(ad)
{
    sn = ad;
    system_dco = 0;
}

UhconnWorkNode::~UhconnWorkNode()
{

}

void UhconnWorkNode::start(void) {
    for (auto it = voxor_map.begin(); it != voxor_map.end(); ++it) {
        // cout << "run voxor:" << it->second.getAddr() << endl;
        it->second.run();
    }
    //co_opt.cycle_timeout_us = 100 * 1000;
    //co_opt.dispatcher_thread_cycle_us = 1000 * 1000;
    //co_opt.debug = 1;
    //std::thread t([]{ co_sched.Start(0); });
    std::thread t([]{ co_sched.Start(0); });
    t.detach();
}

int UhconnWorkNode::inputMessage(UhconnMessage& in_msg) {
    //std::cout <<"inputMessage"<<std::endl;
    if( !isForMe(in_msg) ) {
        std::cout <<"wrongly delivered msg!!"<<std::endl;
        return -1;
    }
    #ifdef DEBUGINFO
    std::cout <<"in_msg.getDestVoxor:"<<in_msg.getDestVoxor()<<std::endl;
    #endif
    std::map<std::string, UhconnVoxor&>::iterator it = voxor_map.find(in_msg.getDestVoxor());
    if(it != voxor_map.end()) {
        it->second.getMsgQ() << in_msg;
    }else{
        std::cout <<"dest Q not found!!"<<std::endl;
    }
    return 0;
}
int UhconnWorkNode::addVoxor(UhconnVoxor& v) {
    voxor_map.insert(std::pair<std::string, UhconnVoxor&>(v.getAddr(), v));
    v.run();
    return 0;
}

bool UhconnWorkNode::isForMe(UhconnMessage& in_msg) {
    return in_msg.getDestNodeId() == itsAddr();
}