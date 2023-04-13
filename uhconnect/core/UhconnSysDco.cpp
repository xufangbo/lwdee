#include "UhconnSysDco.h"
#include "UhconnVoxorFactory.h"
#include "DcoFactory.h"
#include "libgo.h"


std::string UhconnSysDco::create_dco(std::string cname) {
    UhconnDco* dco = UhconnVoxorFactory::getInstance().getLocalFactory()->newDco(cname);
    #ifdef DEBUGINFO
    std::cout << "Create dco " << dco->itsAddr() << "-" << dco->itsRef().itsBelongVoxorId() << std::endl;
    #endif
    return dco->itsRef().itsBelongVoxorId();
}



UhconnSysDco::UhconnSysDco()
{
    getFunctionTable()["create"] = (PTR)&UhconnSysDco::create_dco;

}

UhconnSysDco::~UhconnSysDco()
{

}

co_chan<UhconnMessage> UhconnSysDco::getMsgQ(void) {
    return ch;
}