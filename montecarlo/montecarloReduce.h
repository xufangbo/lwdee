#ifndef MONTECARLOREDUCE_H
#define MONTECARLOREDUCE_H


#include "api/UhconnDco.h"

typedef struct mntcl_result_t{
	unsigned long long count_in;
	unsigned long long count_out;
}mntcl_result;

class montecarloReduce : public UhconnDco{
    public:
    montecarloReduce();
    ~montecarloReduce();
    std::string montecarloRun(std::string a);

    private:
    double montecarlo(std::vector<mntcl_result> &res);
};
#endif
