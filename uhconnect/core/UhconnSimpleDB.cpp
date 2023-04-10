#include "UhconnSimpleDB.h"
#include "UhconnConfig.h"
#include "UhconnVoxorFactory.h"
#include "UhconnWorkNode.h"
#include <memory>
using namespace std;
//#define DEBUGINFO

UhconnSimpleDB::UhconnSimpleDB()
{
    ddoLevel = 1;
    prep_array.clear();
}

UhconnSimpleDB::~UhconnSimpleDB()
{

}

#define DDO_LOCK ddo_mutex.lock()
#define DDO_UNLOCK ddo_mutex.unlock()
#define PREP_LOCK prep_array_mutex.lock()
#define PREP_UNLOCK prep_array_mutex.unlock()
// #define DDO_LOCK 
// #define DDO_UNLOCK 

DdoDataId UhconnSimpleDB::genDataId(){
    static DdoDataId dataId = 0;
    DdoDataId nodeId = UhconnConfig::getInstance().getNodeId();
    DdoDataId genId = nodeId<<48 | ++dataId;
    return genId;
}

// int UhconnSimpleDB::storeBlock(DdoDataId id, DdoBlockData& data){
//     DDO_LOCK;
//     localDdo.insert(std::pair<DdoDataId,DdoBlockData>(id,data));
//     DDO_UNLOCK;
//     return 0;
// }
int UhconnSimpleDB::storeBlock(DdoDataId id, DdoBlockData&& data){
    DDO_LOCK;
    localDdo.insert(std::make_pair(id, std::move(data)));
    DDO_UNLOCK;
    return 0;
}


int UhconnSimpleDB::loadBlock(DdoDataId id, DdoBlockData& data, int nodeId){

    UhconnWorkNode* workNode =  UhconnVoxorFactory::getInstance().getLocalWorkNode();
    workNode->itsRouter().pullData(nodeId, id);
    DDO_LOCK;
    std::map<DdoDataId, DdoBlockData>::iterator it = localDdo.find(id);
    DDO_UNLOCK;
    if(it == localDdo.end()) {
        //std::cout<<"ddo id="<<id<<" not found in sys ddo!!"<<std::endl;
        return -1;
    }

    data = it->second;
    return 0;
}


int UhconnSimpleDB::loadBlock(DdoDataId id, DdoBlockData& data){
    if(!isBlockAtLocal(id)){
        UhconnWorkNode* workNode =  UhconnVoxorFactory::getInstance().getLocalWorkNode();
        workNode->itsRouter().pullData(id);
    }

    DDO_LOCK;
    std::map<DdoDataId, DdoBlockData>::iterator it = localDdo.find(id);
    DDO_UNLOCK;
    if(it == localDdo.end()) {
        //std::cout<<"ddo id="<<id<<" not found in sys ddo!!"<<std::endl;
        return -1;
    }

    data = it->second;
    if( data.isReadOnceOnly) {
        DdoDataId tmp = it->first;
        deleteBlock(tmp);
    }
    return 0;
}


int UhconnSimpleDB::pushBlock(DdoDataId id, DdoBlockData& data){
    data.isReadOnceOnly = true;  
    DDO_LOCK;
    localDdo.insert(std::pair<DdoDataId,DdoBlockData>(id,data));
    DDO_UNLOCK;
    return 0;
}

int UhconnSimpleDB::popBlock(DdoDataId id, DdoBlockData& data){

    int ret = loadBlockFromLocal(id, data);
    if(ret <0){
        return ret;
    }
    ret = deleteBlock(id);
    return ret;
}

int UhconnSimpleDB::loadBlockFromLocal(DdoDataId id, DdoBlockData& data){
    DDO_LOCK;
    std::map<DdoDataId, DdoBlockData>::iterator it = localDdo.find(id);
    DDO_UNLOCK;
    if(it == localDdo.end()) {
        //std::cout<<"ddo id="<<id<<" not found in local ddo!!"<<std::endl;
        return -1;
    }

    data = it->second;
    return 0;
}

bool UhconnSimpleDB::isBlockAtLocal(DdoDataId id){
    DDO_LOCK;
    std::map<DdoDataId, DdoBlockData>::iterator it = localDdo.find(id);
    DDO_UNLOCK;
    if(it == localDdo.end()) {
        return false;
    }
    return true;
}

bool UhconnSimpleDB::isBlockPreparing(const DdoDataId& id) {
    PREP_LOCK;
    bool ret = false;
    for (const auto& it : prep_array) {
        if (it == id) {
           ret = true;
           break;
        }
    }
    PREP_UNLOCK;
    return ret;
}


bool UhconnSimpleDB::addToPrepArray(DdoDataId id){
    PREP_LOCK;
    prep_array.push_back(id);
    #ifdef DEBUGINFO
    std::cout<<"add "<<id << " to array"<<std::endl;
    #endif
    PREP_UNLOCK;
    return false;
}

bool UhconnSimpleDB::delFromPrepArray(DdoDataId id){
    PREP_LOCK;
    for(auto it = prep_array.begin(); it != prep_array.end(); it++){
        if(*it == id){
            prep_array.erase(it);
            #ifdef DEBUGINFO
            std::cout<<"erase "<<id << "from array"<<std::endl;
            #endif
            break;
        }
    }
    PREP_UNLOCK;
    return false;
}

int UhconnSimpleDB::deleteBlock(DdoDataId id){
    DDO_LOCK;
    std::map<DdoDataId, DdoBlockData>::iterator it = localDdo.find(id);
    DDO_UNLOCK;
    if(it == localDdo.end()) {
        return -1;
    }
    DDO_LOCK;
    if(it->second.data) {//if(!it->second.isCopyConstruct){
        free(it->second.data);
        it->second.data = nullptr;
    }
    localDdo.erase(it);
    DDO_UNLOCK;
    return 0;
}

int UhconnSimpleDB::deleteBlockGlobal(DdoDataId id){
    deleteBlock(id);
    UhconnWorkNode* workNode =  UhconnVoxorFactory::getInstance().getLocalWorkNode();
    workNode->itsRouter().deleteData(id);
    return 0;
}

// DdoBlockData* UhconnSimpleDB::createBlock(DdoDataId id,int blockSize,int lifeCycle){
//     if(isBlockAtLocal(id)){
//         std::cout << "block exist at-local "<<id<<std::endl;
//         return getBlockFromLocal(id); 
//     }
//     DdoBlockData data;
//     data.len = blockSize;
//     data.data = malloc(blockSize);
//     if(data.data == NULL){
//         std::cout << "malloc failed while create block"<<std::endl;
//         return nullptr;
//     }
//     memset(data.data, 0, data.len);
//     data.lifeCycle = lifeCycle;
//     storeBlock(id, data);
//     return getBlockFromLocal(id);
// }
DdoBlockData* UhconnSimpleDB::createBlock(DdoDataId id, int blockSize, int lifeCycle) {
    if (isBlockAtLocal(id)) {
        std::cout << "block exist at-local " << id << std::endl;
        return getBlockFromLocal(id);
    }

    std::unique_ptr<DdoBlockData> data(new DdoBlockData());
    data->len = blockSize;
    data->data = std::malloc(blockSize);
    if (data->data == nullptr) {
        std::cout << "malloc failed while create block" << std::endl;
        return nullptr;
    }
    std::memset(data->data, 0, data->len);
    data->lifeCycle = lifeCycle;

    storeBlock(id, std::move(*data));
    return getBlockFromLocal(id);
}

 DdoBlockData* UhconnSimpleDB::getBlockFromLocal(DdoDataId id){
    DDO_LOCK;
    auto it = localDdo.find(id);
    DDO_UNLOCK;
    if(it == localDdo.end()) {
        return nullptr;
    }
    return &(it->second);
}

DdoBlockData* UhconnSimpleDB::getBlock(DdoDataId id){
    int destNodeId = (id>>48) & 0xffff;
    return getBlock(id, destNodeId);
}

DdoBlockData* UhconnSimpleDB::getBlock(DdoDataId id, int destNodeId){
    int ret = 0;
    if(!isBlockAtLocal(id)){
        if(isBlockPreparing(id)){
            #ifdef DEBUGINFO
            std::cout<<"wait block 2" << id << std::endl;
            #endif
            while(isBlockPreparing(id)){
               co_sleep(20);
            }
        }else{
            #ifdef DEBUGINFO
            std::cout<<"to pull data 2" << id << std::endl;
            #endif
            addToPrepArray(id);
            UhconnWorkNode* workNode =  UhconnVoxorFactory::getInstance().getLocalWorkNode();
            if(workNode->itsRouter().pullData(destNodeId, id) < 0) {
                ret = -1;
            }
            delFromPrepArray(id);
        }

    }
    if(ret >= 0){
        return getBlockFromLocal(id);
    }else{
        return nullptr;
    }
    
}

int UhconnSimpleDB::releaseBlock(DdoDataId id){
   DdoBlockData* data = getBlockFromLocal(id);
   if(!data){
    return -1;
   }
   bool isDel = false;
   data->mutex.lock();
   if(data->lifeCycle>0){
      data->lifeCycle--;
      if(data->lifeCycle==0){
         isDel = true; 
      }
   }
   data->mutex.unlock();
   if(isDel){
      deleteBlock(id);
   }
   return 0;
}

int UhconnSimpleDB::shareBlock(DdoDataId id){
    return 0;
}

int UhconnSimpleDB::deleteShareBlock(DdoDataId id){
    return 0;
}

int UhconnSimpleDB::setUpLevelDdoUrl(string& url){
    upLevelDdoUrl = url;
    return 0;
}

DdoDataId UhconnSimpleDB::getFirstDdo(void) {
    DdoDataId rtn = 0;
    DDO_LOCK;
    std::map<DdoDataId, DdoBlockData>::iterator it = localDdo.begin();
    if( it != localDdo.end() ) {
        rtn = it->first;
    }
    DDO_UNLOCK;
    return rtn;
}
DdoDataId UhconnSimpleDB::getNextDdo(DdoDataId cur_ddo) {
    DdoDataId rtn = 0;
    DDO_LOCK;
    std::map<DdoDataId, DdoBlockData>::iterator it = localDdo.find(cur_ddo);
    if( ++it != localDdo.end() ) {
        rtn =  it->first;
    }
    DDO_UNLOCK;
    return rtn;
}
#include <shell_cpp.h>

int ddo_list(void) {
    std::cout << "ddo list:" << std::endl;
    DdoDataId ddoid = UhconnSimpleDB::getInstance().getFirstDdo();
    if( ddoid == 0 ) {
        return 0;
    }
    while(ddoid != 0 ) {
        std::cout << "\t" << ddoid << std::endl;
        ddoid = UhconnSimpleDB::getInstance().getNextDdo(ddoid);
    }
    return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), ddolist, ddo_list, list ddo data);