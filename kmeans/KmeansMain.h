#ifndef KMEANSMAIN_H
#define KMEANSMAIN_H

#pragma once

#include "api/UhconnDco.h"

typedef struct file_depart {
    int start;
    int num;
}fgroup_info;

class KmeansMain : public UhconnDco
{
public:
    KmeansMain();
    ~KmeansMain();

private:

    std::string start(std::string a);
    static void splitFiles(fgroup_info* pif, int group_num, int topFiles = 120);
};

#endif