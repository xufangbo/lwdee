#pragma once

#include "core/DcoFactory.h"

class TerasortDCOFactory : public DcoFactory
{
public:
    TerasortDCOFactory();
    virtual ~TerasortDCOFactory();

    virtual UhconnDco* newDco(std::string& class_name);

private:

};