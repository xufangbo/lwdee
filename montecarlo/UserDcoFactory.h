#ifndef USERDCOFACTORY_H
#define USERDCOFACTORY_H

#pragma once

#include "core/DcoFactory.h"

class UserDcoFactory : public DcoFactory
{
public:
    UserDcoFactory();
    virtual ~UserDcoFactory();

    virtual UhconnDco* newDco(std::string& class_name);

private:

};

#endif