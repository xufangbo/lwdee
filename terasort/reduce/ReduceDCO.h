#pragma once

#include "api/UhconnDco.h"

class ReduceDCO : public UhconnDco
{
public:
    ReduceDCO();
    virtual ~ReduceDCO();

private:
    std::string  reduce(std::string a);
};