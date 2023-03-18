#ifndef USERDCO_H
#define USERDCO_H

#pragma once

#include "api/UhconnDco.h"

class UserDco : public UhconnDco
{
public:
    UserDco();
    virtual ~UserDco();

private:
    std::string  f1(std::string a);
    std::string  f2(std::string a);
};

#endif