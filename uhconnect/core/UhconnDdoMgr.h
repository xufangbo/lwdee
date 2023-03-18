#ifndef UHCONNDDOMGR_H
#define UHCONNDDOMGR_H

#pragma once
#include "UhconnSimpleDB.h"


class UhconnDdoMgr
{
public:
    UhconnDdoMgr();
    ~UhconnDdoMgr();
    static UhconnDdoMgr& getInstance(void) {
        static UhconnDdoMgr instance;
        return instance;
    };
    int pushDdoToRemote(int remoteNodeId, DdoDataId did);
private:

};

#endif