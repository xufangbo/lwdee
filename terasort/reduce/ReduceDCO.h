#pragma once

#include "api/UhconnDco.h"

class ReduceDCO : public UhconnDco
{
public:
    ReduceDCO();
    virtual ~ReduceDCO();

private:
    std::string  f1(std::string a);
    std::string  f2(std::string a);
};