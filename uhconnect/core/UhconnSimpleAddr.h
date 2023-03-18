#ifndef UHCONNSIMPLEADDR_H
#define UHCONNSIMPLEADDR_H

#pragma once

#include "UhconnAddr.h"

class UhconnSimpleAddr : public UhconnAddr
{
    UhconnSimpleAddr() {};
public:
    UhconnSimpleAddr(int worknodeSn, int voxorSn);
    UhconnSimpleAddr(std::string in) {value =in;};
    virtual ~UhconnSimpleAddr();

    virtual std::string& itsValue(void) {
        return value;
    };
    virtual int workNodeSn(void);
    virtual int voxorSn(void);
private:
    std::string value;
};

#endif