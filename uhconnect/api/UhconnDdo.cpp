#include "UhconnDdo.h"
#include <sstream>
#include "log.h"
#include "core/UhconnSimpleAddr.h"
#include "core/UhconnWorkNode.h"
#include "core/UhconnVoxorFactory.h"
#include "log.h"

#ifdef RUN_DEMO
#undef logDebug
#undef logInfo
#undef logError
#define logDebug printf
#define logInfo printf
#define logError printf
#endif

UhconnDdo::~UhconnDdo()
{
    deleteBlock();
}

#ifdef DB_BLOCK_WITHOUT_COPY
int UhconnDdo::storeBlock(DdoBlockData& data) {
    DdoBlockData * ddo;
    ddo = UhconnSimpleDB::getInstance().createBlock(itsRef().itsId(),data.len,0);
    memcpy(ddo->data,data.data,data.len);
    return 0;
    //return UhconnSimpleDB::getInstance().storeBlock(df.itsId(), data);
}                         
#else

UhconnDdo::errno_ddo UhconnDdo::writeData(void* data_, uint64_t len_) {
    if( _wp ) {
        return DDO_WP;  //非创建者，写保护
    }
    UhconnMemoryDB& mdb = UhconnVoxorFactory::getInstance().getLocalWorkNode()->itsDb();
    if( mdb.ifExist(itsRef().itsId()) ) {
        return DDO_MODIFY;  //禁止重复写
    }
    mdb.makeDataCell(itsRef().itsId(), data_, len_);
}
void* UhconnDdo::readData(uint64_t* len_) {

}


int UhconnDdo::storeBlock(DdoBlockData& data) {
    return UhconnSimpleDB::getInstance().storeBlock(df.itsId(), data);
}
#endif //DB_BLOCK_WITHOUT_COPY


int UhconnDdo::loadBlock(DdoBlockData& data) {
    if( !UhconnSimpleDB::getInstance().isBlockAtLocal(df.itsId()) ) {
        //数据不在本地，先pull
        UhconnSimpleAddr vaddr(df.itsVoxorId());
        UhconnWorkNode* workNode =  UhconnVoxorFactory::getInstance().getLocalWorkNode();
        int rst = workNode->itsRouter().pullData(vaddr.workNodeSn(), df.itsId());
        if( rst < 0 ) {
            logError("pull data failed!!");
            return rst;
        }
    }

    return UhconnSimpleDB::getInstance().loadBlock(df.itsId(), data);
}

DdoBlockData* UhconnDdo::getBlock() {
    UhconnSimpleAddr vaddr(df.itsVoxorId());
    UhconnWorkNode* workNode =  UhconnVoxorFactory::getInstance().getLocalWorkNode();
    return UhconnSimpleDB::getInstance().getBlock(df.itsId(), vaddr.workNodeSn());
}

int UhconnDdo::deleteBlock() {
    return UhconnSimpleDB::getInstance().deleteBlock(df.itsId());
}

std::string UhconnDdo::toJsonString(void) {
    std::stringstream ss;
    ss << df.itsId() << " " << df.itsVoxorId();
    return ss.str();
}
