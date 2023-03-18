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

}NodeInfo;

class UhconnWorkNode;
class UhconnRouter
{
public:
    UhconnRouter(UhconnWorkNode* node);
    ~UhconnRouter();
    int sendMsg(UhconnMessage& msg);
    int pullData(int destNodeId, DdoDataId ddoId);
    int pullData(DdoDataId ddoId);
    int pullData2(int destNodeId, DdoDataId ddoId);
    int pullData2(DdoDataId ddoId);
    int pushData(int destNodeId, DdoDataId ddoId);
    int deleteData(int destNodeId, DdoDataId ddoId);
    int deleteData(DdoDataId ddoId);
    int regNode(int id, NodeInfo info);
    int setupRouteInfoFromConf(void);
    UhconnWorkNode * getWorkNode();
    virtual int nodeInfo(int id,NodeInfo& info);

public:
    static UhconnTCPServer tcpMsgServer;
    static UhconnTCPServer tcpDataServer;
    //static UhconnTCPClient tcpClient;  
    
private:
    std::map<int,NodeInfo> nodeMap;
    UhconnWorkNode *workNode;  

};

#endif