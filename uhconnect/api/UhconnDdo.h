#ifndef UHCONNDDO_H
#define UHCONNDDO_H

#pragma once

#include "core/UhconnSimpleDB.h"
#include "api/UhconnDdoRef.h"
#include <string>

class UhconnDdoRef;
class UhconnDdo
{
    UhconnDdo();
    UhconnDdo(UhconnDdo&);
public:
    UhconnDdo(UhconnDdoRef& ddorf, bool wp_ = false) : df(ddorf), _wp(wp_) {};
    ~UhconnDdo();

    typedef enum {
        DDO_SUCCESS,
        DDO_MODIFY,
        DDO_WP,
    }errno_ddo;
    errno_ddo writeData(void* data_, uint64_t len_);
    void* readData(uint64_t* len_);

    //以下接口将被删除
    int storeBlock(DdoBlockData& data);
    int loadBlock(DdoBlockData& data);
    int deleteBlock();
    DdoBlockData* getBlock();
    std::string toJsonString(void);
    UhconnDdoRef& itsRef() { return df;};
    void setWriteProtect(void) {_wp = true;};
private:
    UhconnDdoRef df;
    bool _wp;       //写保护，非本地创建的为true，在wait时处理
};

#endif