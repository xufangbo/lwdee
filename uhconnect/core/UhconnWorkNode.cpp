#include "UhconnWorkNode.h"
#include "UhconnVoxor.h"

#include <libgo.h>
#include <iostream>
#include <string>

using namespace std;
UhconnWorkNode::UhconnWorkNode(int ad)
    : scheduler(ad), router(this), _db(ad) {
  sn = ad;
  system_dco = 0;
}

UhconnWorkNode::~UhconnWorkNode() {
}

void UhconnWorkNode::start(void) {
  for (auto it = voxor_map.begin(); it != voxor_map.end(); ++it) {
    // cout << "run voxor:" << it->second.getAddr() << endl;
    it->second.run();
  }
  // co_opt.cycle_timeout_us = 100 * 1000;
  // co_opt.dispatcher_thread_cycle_us = 1000 * 1000;
  // co_opt.debug = 1;
  // std::thread t([]{ co_sched.Start(0); });
  std::thread t([] { co_sched.Start(0); });
  t.detach();
}

int UhconnWorkNode::inputMessage(UhconnMessage& in_msg) {
  // std::cout <<"inputMessage"<<std::endl;
  if (!isForMe(in_msg)) {
    // std::cout <<"wrongly delivered msg!!"<<std::endl;
    return -1;
  }
  // std::cout <<"in_msg.getMsgId:"<<in_msg.getMsgId()<<std::endl; throw std::range_error("missing watied msg");

  if (isWaitResponse(in_msg) && isMsgInWaitingTable(in_msg)) {  // client
    auto channel = getFromWaitingTable(in_msg);
    if (channel == nullptr) {
      throw std::range_error("missing watied msg in UhconnWorkNode::inputMessage," + std::to_string(in_msg.getMsgId()));
    }
    assert(channel != nullptr);
    channel->push(in_msg);
  } else {  // server
    std::map<std::string, UhconnVoxor&>::iterator it = voxor_map.find(in_msg.getDestVoxor());
    if (it != voxor_map.end()) {
      // std::cout << "accept message,----------------------------------------- " << it->second.docName() << "::" << in_msg.getMethodName() << std::endl;
      it->second.getMsgQ() << in_msg;
    } else {
      std::cout << "dest Q not found!!" << std::endl;
    }
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

bool UhconnWorkNode::isWaitResponse(UhconnMessage& in_msg) {
  return (in_msg.getType() == MSG_TYPE_RESP) && (in_msg.getCmd() == MSG_CMD_WAIT || in_msg.getCmd() == MSG_CMD_CREATE);
}

bool UhconnWorkNode::isMsgInWaitingTable(UhconnMessage& in_msg) {
  size_t msg_hash = in_msg.getMsgId();
  auto iter = waiting_table.find(msg_hash);
  if (iter != waiting_table.end()) {
    return true;
  } else {
    return false;
  }
}

co_chan<UhconnMessage>* UhconnWorkNode::addToWaitingTable(UhconnMessage& in_msg) {

  size_t msg_hash = std::hash<UhconnMessage>()(in_msg);

  in_msg.setMsgId(msg_hash);
  auto channel = new co_chan<UhconnMessage>(10);
  std::lock_guard<std::mutex> lock(waiting_table_mutex);
  waiting_table[msg_hash] = channel;
  // return std::move(channel);
  // std::cout << "add waiting table : " << msg_hash << std::endl;
  return channel;
}

co_chan<UhconnMessage>* UhconnWorkNode::getFromWaitingTable(const UhconnMessage& msg) {
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
  auto it = waiting_table.find(msg.getMsgId());
  if (it != waiting_table.end()) {
    delete it->second;
  }
  waiting_table.erase(msg_hash);
  return true;
}