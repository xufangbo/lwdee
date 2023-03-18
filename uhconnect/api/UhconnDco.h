#ifndef UHCONNDCO_H
#define UHCONNDCO_H

#pragma once

#include <map>
#include <functional>
#include <string>
#include "UhconnDcoRef.h"
#include "api/UhconnDdoRef.h"
#include "libgo.h"
class UhconnDco;

typedef std::string(UhconnDco::*PTR)(std::string);
typedef std::map<std::string, PTR> FunctionTable;

class UhconnVoxor;
class UhconnMessage;
class UhconnDdo;
class UhconnDco
{
public:
    UhconnDco();
    virtual ~UhconnDco();

    std::string& itsAddr(void);
    UhconnDcoRef create(std::string className);
    UhconnDcoRef create(std::string className, std::string classConstructor);
    const UhconnDdoRef async(UhconnDcoRef& dco, std::string funcName, std::string funcPara);
    UhconnDdoRef async(UhconnDcoRef& dco, std::string reqFuncName, UhconnDdo* reqPara);
    UhconnDdo* wait(UhconnDdoRef& ddo_id);
    std::vector<UhconnDdo*> wait(std::vector<UhconnDdoRef>& ddo_ref, bool postIfReady);

    virtual co_chan<UhconnMessage> getMsgQ(void);

    virtual FunctionTable& getFunctionTable(void) {
        return ft;
    };
    UhconnDcoRef& itsRef(void) {return *df;};
    int itsWorkNodeSn(void);
private:
    UhconnVoxor* itsVoxor;
    UhconnDcoRef* df;
    FunctionTable ft;
    std::string wait_ddo(std::string ddo_id);
};

#endif