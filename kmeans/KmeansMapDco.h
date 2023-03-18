#ifndef KMEANSMAPDCO_H
#define KMEANSMAPDCO_H

#pragma once

#include "api/UhconnDco.h"

class KMeansMap;
class KmeansMapDco : public UhconnDco
{
public:
    KmeansMapDco();
    ~KmeansMapDco();

private:
    KMeansMap* kmap;
    std::string init(std::string a);
    std::string init1(std::string a);
    std::string init2(std::string a);
    std::string runOneStep(std::string a);
    std::string runOneStep2(std::string a);
    std::string set_means(std::string a);
    std::string set_means2(std::string a);

};

#endif