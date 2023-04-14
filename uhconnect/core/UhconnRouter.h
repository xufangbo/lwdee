#ifndef UHCONNROUTER_H
#define UHCONNROUTER_H

#pragma once

#include <map>
#include "core/UhconnMessage.h"
#include "UhconnTCPServer.h"
#include "UhconnTCPClient.h"
#include "UhconnSimpleDB.h"
#include "libgo.h"



#define ROUTER_MAX_DATA_PKT_LEN (64*1024)

#define ROUTER_DATA_NOT_FOUND 0xBADCBADC

typedef struct NodeInfo_t{
    std::string ipv4Addr;
    int nodeId;
    unsigned short msgPort;
    unsigned short dataPort;
    UhconnTCPClient * msgClient;
    UhconnTCPClient * dataClient;
    co_mutex *msgLock;
    co_mutex *dataLock;
    void setMsgClient(UhconnTCPClient *client) {
        msgLock->lock();
        if(msgClient == nullptr){
            msgClient = client;
        } else{
            delete msgClient;
            msgClient = client;
        }
        msgLock->unlock();
    };
}NodeInfo;

class co_mutex_lock {
public:
    co_mutex_lock(co_mutex& m) : m_(m) { m_.lock(); }
    ~co_mutex_lock() { m_.unlock(); }
private:
    co_mutex& m_;
};

class UhconnWorkNode;
class UhconnRouter
{
public:
    UhconnRouter(UhconnWorkNode* node);
    ~UhconnRouter();
    int sendMsg(UhconnMessage& msg);
    int pullData(int destNodeId, DdoDataId ddoId);
    int pullData(DdoDataId ddoId);

    int pushData(int destNodeId, DdoDataId ddoId);
    int deleteData(int destNodeId, DdoDataId ddoId);
    int deleteData(DdoDataId ddoId);
    int regNode(int id, const NodeInfo& info);
    int setupRouteInfoFromConf(void);
    UhconnWorkNode * getWorkNode();
    virtual int nodeInfo(int id,NodeInfo& info);

public:

    //static UhconnTCPClient tcpClient;  
    int msgRxHandler(char* data, int fd, int len);
    int dataRxHandler(char* data, int fd, int len);    
private:
    co_mutex mapLock;
    std::map<int,NodeInfo> nodeMap;
    UhconnWorkNode *workNode;  
    NodeInfo& getNodeInfo(int destNodeId);
    int receive_block(UhconnTCPClient* client, DdoDataId ddoId);

    UhconnTCPServer tcpMsgServer;
    UhconnTCPServer tcpDataServer;
    enum {
        ok = 0,
        err_node = -1,
        err_client = -2,
        err_send = -3,
        err_rcv = -4,
        err_nodata = -5,
        err_datalen = -6,
    };
};

#endif