#ifndef UHCONNDCOREF_H
#define UHCONNDCOREF_H

#pragma once

#include <string>

class UhconnDcoRef
{
public:
    UhconnDcoRef();
    UhconnDcoRef(const std::string& belong);
    ~UhconnDcoRef();

    const std::string& itsBelongVoxorId(void){
        return voxorId;
    };

    int itsBelongWorkNodeId(void);

private:
    std::string voxorId;
};

#endif