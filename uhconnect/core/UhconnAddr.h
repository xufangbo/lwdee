#ifndef UHCONNADDR_H
#define UHCONNADDR_H

#pragma once

#include <string>

class UhconnAddr
{
public:
    UhconnAddr();
    virtual ~UhconnAddr();

    virtual std::string& itsValue(void) = 0;
private:

};

#endif