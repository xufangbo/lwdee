#ifndef DCOFACTORY_H
#define DCOFACTORY_H

#pragma once

#include <string>

class UhconnDco;
class DcoFactory
{
public:
    DcoFactory();
    virtual ~DcoFactory();

    virtual UhconnDco* newDco(std::string& class_name) = 0;
private:

};

#endif