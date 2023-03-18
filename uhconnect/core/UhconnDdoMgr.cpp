#include "UhconnDdoMgr.h"
#include "UhconnRouter.h"
#include "UhconnConfig.h"
#include "UhconnVoxorFactory.h"
#include "UhconnWorkNode.h"
#include <iostream>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include "libgo.h"

UhconnDdoMgr::UhconnDdoMgr()
{

}

UhconnDdoMgr::~UhconnDdoMgr()
{

}

int UhconnDdoMgr::pushDdoToRemote(int remoteNodeId, DdoDataId did){
    int ret = 0;
    UhconnWorkNode* workNode =  UhconnVoxorFactory::getInstance().getLocalWorkNode();
    ret = workNode->itsRouter().pushData(remoteNodeId, did);
    return ret;
}