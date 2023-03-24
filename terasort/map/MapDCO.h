#pragma once

#include "api/UhconnDco.h"

class MapDCO : public UhconnDco
{
public:
    MapDCO();
    virtual ~MapDCO();

private:
    std::string  f1(std::string a);
    std::string  f2(std::string a);
};