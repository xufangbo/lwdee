#include "UhconnWorkNode.h"
#include "UhconnVoxor.h"

#include <iostream>
#include <libgo.h>
#include <string>
#include <atomic>
#include "core/log.hpp"

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
    logger_trace("in_msg.getMsgId:%lld",in_msg.getMsgId());

    if( isWaitResponse(in_msg) && isMsgInWaitingTable(in_msg) ) {
        logger_trace("yes isMsgInWaitingTable");
        auto channel = getFromWaitingTable(in_msg);
        assert(channel);
        channel->push(in_msg);
    }
    else {
        std::lock_guard<std::mutex> lock(voxor_map_mutex);
        std::map<std::string, UhconnVoxor&>::iterator it = voxor_map.find(in_msg.getDestVoxor());
        if(it != voxor_map.end()) {
            it->second.getMsgQ() << in_msg;
        }else{
            std::cout <<"dest Q not found!!"<<std::endl;
        }
    }
    return 0;
}
int UhconnWorkNode::addVoxor(UhconnVoxor& v) {
    std::lock_guard<std::mutex> lock(voxor_map_mutex);
    voxor_map.insert(std::pair<std::string, UhconnVoxor&>(v.getAddr(), v));
    v.run();
    return 0;
}

bool UhconnWorkNode::isForMe(UhconnMessage& in_msg) {
    return in_msg.getDestNodeId() == itsAddr();
}

bool UhconnWorkNode::isWaitResponse(UhconnMessage& in_msg) {
    return (in_msg.getType() == MSG_TYPE_RESP) && (in_msg.getCmd() == MSG_CMD_WAIT || in_msg.getCmd() == MSG_CMD_CREATE);
}

bool UhconnWorkNode::isMsgInWaitingTable(UhconnMessage& in_msg) {
    size_t msg_hash = in_msg.getMsgId();
    std::lock_guard<std::mutex> lock(waiting_table_mutex);
    auto iter = waiting_table.find(msg_hash);
    if (iter != waiting_table.end()) {
        return true;
    } else {
        return false;
    }
}


std::shared_ptr<co_chan<UhconnMessage>> UhconnWorkNode::addToWaitingTable(UhconnMessage& in_msg) {
    size_t msg_hash = std::hash<UhconnMessage>()(in_msg);
    in_msg.setMsgId(msg_hash);
    auto channel = std::make_shared<co_chan<UhconnMessage>>(10);
    std::lock_guard<std::mutex> lock(waiting_table_mutex);
    waiting_table[msg_hash] = channel;
    return std::move(channel);
}

std::shared_ptr<co_chan<UhconnMessage>> UhconnWorkNode::getFromWaitingTable(const UhconnMessage& msg) {
    std::lock_guard<std::mutex> guard(waiting_table_mutex);
    auto it = waiting_table.find(msg.getMsgId());
    if (it != waiting_table.end()) {
        return it->second;
    }
    return nullptr;
}
bool UhconnWorkNode::removeFromWaitingTable(UhconnMessage& msg) {
    size_t msg_hash = msg.getMsgId();
    std::lock_guard<std::mutex> lock(waiting_table_mutex);
    waiting_table.erase(msg_hash);    
    return true;
}