#ifndef KMEANSREDUCEDCO_H
#define KMEANSREDUCEDCO_H

#pragma once

#include "api/UhconnDco.h"

class KmeansReduceDco : public UhconnDco
{
public:
    KmeansReduceDco();
    ~KmeansReduceDco();

private:
    std::string  init(std::string a);
    std::string measFromStepResult(std::string a);
    std::string measFromStepResult2(std::string a);
};

#endif