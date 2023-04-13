#ifndef UHCONNSIMPLEDB_H
#define UHCONNSIMPLEDB_H

#include <iostream>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include "libgo.h"

#pragma once

typedef unsigned long long DdoDataId;
#ifdef DB_BLOCK_WITHOUT_COPY
typedef struct DdoBlockData_t
{
    DdoBlockData_t() {
        lifeCycle = 0;
        type = 0;
        len = 0;
        isCopyConstruct = false;
        isReadOnceOnly = false;
        data = nullptr;
    };
    DdoBlockData_t(const DdoBlockData_t& in_data)  {
        len = in_data.len;
        type = in_data.type;
        data = in_data.data;
        lifeCycle = in_data.lifeCycle;
        isCopyConstruct = false;
        isReadOnceOnly = false;
    };
    ~DdoBlockData_t() {
    };

    DdoBlockData_t operator=(const DdoBlockData_t& in_data){
        this->len = in_data.len;
        this->type = in_data.type;
        this->data = in_data.data;
        this->lifeCycle = in_data.lifeCycle;
        this->isCopyConstruct = in_data.isCopyConstruct;
        this->isReadOnceOnly = in_data.isReadOnceOnly;
        return *this;
    };
    int type;
    unsigned int len;
    void * data;
    int lifeCycle;
    bool isCopyConstruct; // should be deleted
    bool isReadOnceOnly; // should be deleted
    co_mutex mutex;
}DdoBlockData;
#else
typedef struct DdoBlockData_t
{
    DdoBlockData_t() {
        isCopyConstruct = false;
        isReadOnceOnly = false;
        lifeCycle = 0;
    };
    DdoBlockData_t(const DdoBlockData_t& in_data)  {
        len = in_data.len;
        type = in_data.type;
        data = malloc(in_data.len);
        memcpy(data, in_data.data, in_data.len);
        isCopyConstruct = true;
        isReadOnceOnly = in_data.isReadOnceOnly;
    };

    DdoBlockData_t operator=(const DdoBlockData_t& in_data){
        this->len = in_data.len;
        this->type = in_data.type;
        this->data = malloc(in_data.len);
        memcpy(this->data, in_data.data, in_data.len);
        this->isCopyConstruct = true;
        this->isReadOnceOnly = in_data.isReadOnceOnly;
        return *this;
    };

    ~DdoBlockData_t() {
        if( data && isCopyConstruct) {
            free(data);
            data = nullptr;
        }
    };
    int type;
    unsigned int len;
    void * data;

    bool isCopyConstruct;
    bool isReadOnceOnly;
    int lifeCycle;
    co_mutex mutex;
}DdoBlockData;

#endif //DB_BLOCK_WITHOUT_COPY
 
class UhconnSimpleDB
{
    UhconnSimpleDB();
    ~UhconnSimpleDB();
    UhconnSimpleDB(const UhconnSimpleDB&);
    UhconnSimpleDB& operator = (const UhconnSimpleDB&); 
public:
    //@brief 单例模式，c++11下线程安全
    static UhconnSimpleDB& getInstance(void) {
        static UhconnSimpleDB instance;
        return instance;
    };

    DdoDataId genDataId();
    int storeBlock(DdoDataId id, DdoBlockData&& data);
    int loadBlock(DdoDataId id, DdoBlockData& data);
    int loadBlock(DdoDataId id, DdoBlockData& data, int nodeId);
    int loadBlockFromLocal(DdoDataId id, DdoBlockData& data);
    int pushBlock(DdoDataId id, DdoBlockData& data);
    int popBlock(DdoDataId id, DdoBlockData& data);
    DdoBlockData* createBlock(DdoDataId id,int blockSize,int lifeCycle=0);
    DdoBlockData* getBlockFromLocal(DdoDataId id);
    DdoBlockData* getBlock(DdoDataId id);
    DdoBlockData* getBlock(DdoDataId id, int destNodeId);
    int releaseBlock(DdoDataId id);
    bool isBlockAtLocal(DdoDataId id);
    int deleteBlock(DdoDataId id);
    int deleteBlockGlobal(DdoDataId id);
    int shareBlock(DdoDataId id);
    int deleteShareBlock(DdoDataId id);
    int setUpLevelDdoUrl(std::string& url);
    bool isBlockPreparing(const DdoDataId& id);
    bool addToPrepArray(DdoDataId id);
    bool delFromPrepArray(DdoDataId id);
    DdoDataId getFirstDdo(void);
    DdoDataId getNextDdo(DdoDataId cur_ddo);
private:

    std::map<DdoDataId, DdoBlockData> localDdo;
    std::vector<DdoDataId> prep_array;
    std::string upLevelDdoUrl;
    co_mutex ddo_mutex;
    co_mutex prep_array_mutex;
    int ddoLevel;
};

#endif