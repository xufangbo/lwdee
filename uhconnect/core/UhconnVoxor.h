#ifndef UHCONNVOXOR_H
#define UHCONNVOXOR_H

#pragma once

#include "UhconnSimpleAddr.h"
#include "libgo.h"
#include "api/UhconnDco.h"

class UhconnWorkNode;
class UhconnMessage;
class UhconnDco;
class UhconnVoxor
{
    UhconnVoxor();
public:
    UhconnVoxor(UhconnWorkNode* worknod, UhconnDco* dco, int sn);
    ~UhconnVoxor();

    std::string& getAddr(void);
    co_chan<UhconnMessage>& getMsgQ(void);
    void run(void);
    UhconnWorkNode* itsBelongToWorkNode(void) {
        return belongTo;
    };

    std::string docName(){return ItsDco->name;};
private:
    UhconnSimpleAddr ItsAddr;
    UhconnWorkNode* belongTo;
    co_chan<UhconnMessage> MsgQ;
    UhconnDco* ItsDco;

};

#endif