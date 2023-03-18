#ifndef UHCONNSYSDCO_H
#define UHCONNSYSDCO_H

#pragma once

#include "api/UhconnDco.h"
#include "UhconnMessage.h"

class UhconnSysDco : public UhconnDco
{
public:
    UhconnSysDco();
    ~UhconnSysDco();
    virtual co_chan<UhconnMessage> getMsgQ(void);
private:

    co_chan<UhconnMessage> ch;
    std::string create_dco(std::string cname);
};

#endif