#ifndef UHCONNAPI_H
#define UHCONNAPI_H

#pragma once

#include <string>
#include "core/UhconnSimpleDB.h"

class UhconnDcoRef;
class UhconnDdoRef;
class UhconnDdo;
class UhconnApi
{
public:
    UhconnApi();
    ~UhconnApi();

    static UhconnDcoRef create(std::string className);
    static UhconnDcoRef create(std::string className, std::string classConstructor);

    static const UhconnDdoRef async(UhconnDcoRef& dco, std::string funcName, std::string funcPara);
    static UhconnDdoRef async(UhconnDcoRef& dco, std::string reqFuncName, UhconnDdo* reqPara);    

    static UhconnDdo* wait(UhconnDdoRef& ddo_id);
    static std::vector<UhconnDdo*> wait(std::vector<UhconnDdoRef>& ddo_ref, bool postIfReady);
    static DdoBlockData loadData(UhconnDdoRef& ddo_id);
      
private:

};

#endif