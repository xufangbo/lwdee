#pragma once

#include "core/DcoFactory.h"

class DscDcoFactory : public DcoFactory
{
public:
    DscDcoFactory();
    virtual ~DscDcoFactory();

    virtual UhconnDco* newDco(std::string& class_name);

private:

};