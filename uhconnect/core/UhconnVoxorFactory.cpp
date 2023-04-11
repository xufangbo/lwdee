#include "UhconnVoxorFactory.h"
#include "UhconnVoxor.h"
#include "UhconnWorkNode.h"
#include "api/UhconnDco.h"
#include "DcoFactory.h"
#include <atomic>

UhconnVoxorFactory::UhconnVoxorFactory()
{

}

UhconnVoxorFactory::~UhconnVoxorFactory()
{

}

// static UhconnWorkNode* Local_Worknode;
// static DcoFactory* local_factory;
void UhconnVoxorFactory::setupLocalWorkEnvironment(DcoFactory* df, int sn) {
    static int Work_Node_Cnt = 80000;
    if( sn == 0 ) {
        sn = ++Work_Node_Cnt;
    }
    // std::cout<<"create work node id:"<<sn<<std::endl;
    Local_Worknode = new UhconnWorkNode(sn);
    Local_Worknode->setSysDco(new UhconnSysDco());
    local_factory = df;
    Local_Worknode->start();
}


UhconnVoxor* UhconnVoxorFactory::createVoxor(UhconnDco* dco) {
    static std::atomic<int> addr_count(0);
    //@todo voxor加入到本地worknode
    UhconnVoxor* v = new UhconnVoxor(getLocalWorkNode(), dco, addr_count++);
    #ifdef DEBUGINFO
    std::cout<<"createVoxor:"<<v->getAddr()<<std::endl;
    #endif
    Local_Worknode->addVoxor(*v);
    return v;
}


UhconnWorkNode* UhconnVoxorFactory::getLocalWorkNode(void) {
    return Local_Worknode;
}
DcoFactory* UhconnVoxorFactory::getLocalFactory(void) {
    return local_factory;
}

void UhconnVoxorFactory::setLocalFactory(DcoFactory* nf) {
    if( local_factory ) {
        delete local_factory;
    }
    local_factory = nf;
}

DcoFactory* UhconnVoxorFactory::switchLocalFactory(DcoFactory* nf) {
    DcoFactory* old = local_factory;
    if( nf ) {
        local_factory = nf;
    }
    return old;
}