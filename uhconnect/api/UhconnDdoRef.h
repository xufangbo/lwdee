#ifndef UHCONNDDOREF_H
#define UHCONNDDOREF_H

#pragma once

#include "core/UhconnSimpleDB.h"

class UhconnDdoRef
{
public:
    UhconnDdoRef();
    UhconnDdoRef(const std::string& vid, DdoDataId did = 0) {
        if( did == 0 ) {
            id = UhconnSimpleDB::getInstance().genDataId();
        }
        else {
            id = did;
        }
        voxorId = vid;
    };
    UhconnDdoRef(const UhconnDdoRef& fid) {
        id = fid.id;
        voxorId = fid.voxorId;
    };
    ~UhconnDdoRef();

    DdoDataId& itsId() {
        return id;
    };

    std::string& itsVoxorId() {
        return voxorId;
    };
    int itsWorkNodeId(void);
    void setDdoId(DdoDataId id_) {
        id = id_;
    };
private:
    DdoDataId id;
    std::string voxorId;
};

#endif