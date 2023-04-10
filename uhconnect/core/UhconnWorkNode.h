#ifndef UHCONNWORKNODE_H
#define UHCONNWORKNODE_H

#pragma once

#include <map>
#include <unordered_map>
#include "UhconnScheduler.h"
#include "UhconnSysDco.h"
#include "UhconnRouter.h"
#include "UhconnMsgParser.h"
#include "UhconnMemoryDB.h"

class UhconnMessage;
class UhconnVoxor;

class UhconnWorkNode
{
    UhconnWorkNode();
public:

    UhconnWorkNode(int ad);
    ~UhconnWorkNode();

    void start(void);
    int inputMessage(UhconnMessage& in_msg);
    int addVoxor(UhconnVoxor& voxor);
    int itsAddr(void) {return sn;};
    UhconnScheduler& itsScheduler(void) {return scheduler;};
    UhconnRouter& itsRouter(void) {return router;};
    UhconnSysDco& itsDeamon(void) {return *system_dco;};
    UhconnMemoryDB& itsDb(void) {return _db;};
    int itId(void) {return sn;};
    void setSysDco(UhconnSysDco* d) {system_dco = d;};

    std::shared_ptr<co_chan<UhconnMessage>> addToWaitingTable(UhconnMessage& in_msg);
    bool removeFromWaitingTable(UhconnMessage& msg);
    std::shared_ptr<co_chan<UhconnMessage>> getFromWaitingTable(const UhconnMessage& msg);
private:
    int sn;     //从1开始
    std::map<std::string, UhconnVoxor&> voxor_map;
    UhconnSysDco* system_dco;
    UhconnScheduler scheduler;
    UhconnRouter router;
    UhconnMemoryDB _db;
    bool isForMe(UhconnMessage&);
    bool isWaitResponse(UhconnMessage&);
    bool isMsgInWaitingTable(UhconnMessage&);
    std::mutex waiting_table_mutex;
    std::unordered_map<size_t, std::shared_ptr<co_chan<UhconnMessage>>> waiting_table;
};

#endif