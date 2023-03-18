#ifndef MONTECARLOMAP_H
#define MONTECARLOMAP_H

#include "api/UhconnDco.h"

class montecarloMap : public UhconnDco{
    public:
    montecarloMap();
    ~montecarloMap();
    std::string montecarloRun(std::string a);
    private:
    // static int montecarlo(unsigned long long &points,
    //                unsigned long long &count_in,
	// 			   unsigned long long &count_out);
    //int montecarlo(unsigned int dco_idx);
};

#endif