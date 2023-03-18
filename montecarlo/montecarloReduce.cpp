#include <iostream>
#include <sstream>
#include <random>
#include "core/UhconnSimpleDB.h"
#include "montecarloReduce.h"


// void error_callback(int error, const char* description) {
// 	fprintf(stderr, "Error: %s\n", description);
// }


double montecarloReduce::montecarlo(std::vector<mntcl_result> &res) {
	
	unsigned long long count_in = 0;
	unsigned long long count_out = 0;

    for(auto it:res){
       count_in+=it.count_in;
	   count_out+=it.count_out;
	}
	return (double)count_in / (double)(count_out + count_in) * 4;
}


std::string montecarloReduce::montecarloRun(std::string a){
	 std::stringstream ss(a);
    size_t k;
    ss >> k;
    std::vector<DdoDataId> mid_data_id(k);
	std::vector<mntcl_result> mid_data(k);
	mntcl_result mid_res;
    // std::cout << "k=" <<k <<std::endl;
    for (size_t i = 0; i < k; i++)
    {
        ss >> mid_data_id[i];
        DdoBlockData block;
        UhconnSimpleDB::getInstance().loadBlock(mid_data_id[i], block);
		unsigned long long *p = static_cast<unsigned long long *>(block.data);
		
		mid_res.count_in = *p;
		mid_res.count_out = *(p+1);
		//std::cout<<"reduce: count_in-count_out = "<<mid_res.count_in<<" - "<<mid_res.count_out<<std::endl;
		mid_data[i] = mid_res;
    }
    double pi = montecarlo(mid_data);
    std::cout<<"reduce: pi = "<< pi<< std::endl;
    std::stringstream asss;
    asss << pi;
	
    return asss.str();

}


montecarloReduce::montecarloReduce()
{
    getFunctionTable()["montecarlo"] = (PTR)&montecarloReduce::montecarloRun;
}

montecarloReduce::~montecarloReduce(){

}