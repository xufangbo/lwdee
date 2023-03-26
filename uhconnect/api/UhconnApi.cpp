#include "UhconnApi.h"
#include "core/UhconnVoxorFactory.h"
#include "core/UhconnWorkNode.h"
#include "api/UhconnDdo.h"

UhconnApi::UhconnApi()
{

}

UhconnApi::~UhconnApi()
{

}

UhconnDcoRef UhconnApi::create(std::string className) {
    UhconnWorkNode* localNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
    return localNode->itsDeamon().create(className);
}

UhconnDcoRef UhconnApi::create(int nodeId,std::string className) {
    UhconnWorkNode* localNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
    return localNode->itsDeamon().create(nodeId,className);
}

UhconnDcoRef UhconnApi::create(std::string className, std::string classConstructor) {
    UhconnWorkNode* localNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
    return localNode->itsDeamon().create(className, classConstructor);
}


const UhconnDdoRef UhconnApi::async(UhconnDcoRef& dco, std::string funcName, std::string funcPara) {
   

    UhconnWorkNode* localNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
    return localNode->itsDeamon().async(dco, funcName,funcPara);
}
UhconnDdo* UhconnApi::wait(UhconnDdoRef& ddo_id) {
    UhconnWorkNode* localNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
    return localNode->itsDeamon().wait(ddo_id);    
}

std::vector<UhconnDdo*> UhconnApi::wait(std::vector<UhconnDdoRef>& ddo_ref, bool postIfReady) {
    UhconnWorkNode* localNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
    return localNode->itsDeamon().wait(ddo_ref,postIfReady);    
}

UhconnDdoRef UhconnApi::async(UhconnDcoRef& dco, std::string reqFuncName, UhconnDdo* reqPara) {
    UhconnWorkNode* localNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
    return localNode->itsDeamon().async(dco, reqFuncName,reqPara);    
}    

DdoBlockData UhconnApi::loadData(UhconnDdoRef& ddo_id) {
    DdoBlockData data;
    wait(ddo_id)->loadBlock(data);
    return data;
}