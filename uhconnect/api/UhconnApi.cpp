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

UhconnDcoRef UhconnApi::create(std::string className, int destnode) {
    UhconnWorkNode* localNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
    return localNode->itsDeamon().create(className, destnode);
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

UhconnDdo* UhconnApi::makeLocalDdo(void* data, uint32_t len) {
    UhconnDdoRef df(UhconnVoxorFactory::getInstance().getLocalWorkNode()->itsDeamon().itsAddr());
    UhconnDdo* ddo_obj = new UhconnDdo(df);
    DdoBlockData test_data;
    test_data.type = 22;
    test_data.len = len;
    test_data.data = data; 
    ddo_obj->storeBlock(test_data);
    return ddo_obj;
}