#include <iostream>
#include <sstream>
#include <sys/time.h>
//#include "UserDco.h"
#include "api/UhconnDdo.h"
#include "api/UhconnDdoRef.h"
#include "api/UhconnApi.h"
#include "core/UhconnSimpleDB.h"

#include "core/UhconnRouter.h"
#include "core/UhconnWorkNode.h"

#ifdef RUN_DEMO
#include "core/UhconnConfig.h"
#include "core/UhconnVoxorFactory.h"
#include "core/UhconnRouter.h"
#include "UserDcoFactory.h"
#include <map>
#endif  

#define AUTO_RUN 1

#ifndef RUN_DEMO
int montecarlo_run(int argc, char *argv[]) {
#else
int main(int argc, char *argv[]) {
#endif   
	long long points;
    long long points_per_dco;
    size_t Dco_k =  atoi(argv[1]);  
    int  nodeAmount =  atoi(argv[2]); 

#ifdef RUN_DEMO
    const std::string confPath = argv[3];
    const std::string nodeName = argv[4];

    if(UhconnConfig::getInstance().loadConf(confPath,nodeName) != 0){
        std::cout<<"load config failed!!"<<endl;
    }
    if(nodeAmount>0){
        UhconnConfig::getInstance().setNodeAmount(nodeAmount);
    }
    UhconnVoxorFactory::getInstance().setupLocalWorkEnvironment(new UserDcoFactory(),UhconnConfig::getInstance().getNodeId()); 
    UhconnWorkNode* workNode = getLocalWorkNode();
    workNode->itsRouter().setupRouteInfoFromConf();
    if(nodeName.compare("node1")){
    while(1){
        usleep(10000);
    }
   }

#endif
	std::cout << "Insert the number of points you want to generate:" << std::endl;
    #if AUTO_RUN
    points = 500000000000;
    #else
	std::cin >> points;
    #endif
    points_per_dco = points/Dco_k;
	std::ostringstream stream;
	stream << "Monte Carlo method with " << points << " points,"<< points_per_dco <<"for each DCO"<<std::endl;

    struct timeval t1,t2,t3,t4;
    double timeuse1, timeuse2;
    gettimeofday(&t1,NULL);
   // std::cout << "step 1 create" << std::endl;
    UhconnDcoRef* map_dcos = new UhconnDcoRef[Dco_k];
    std::vector<UhconnDdoRef> ddo_ids(Dco_k);
   for (size_t i = 0; i < Dco_k; i++)
    {
        map_dcos[i] = UhconnApi::create(std::string("montecarloMap"));
    }
    UhconnDcoRef reduce_dco = UhconnApi::create(std::string("montecarloReduce"));
    gettimeofday(&t2,NULL);
    timeuse1 = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    std::cout << "all dcos create done by " <<timeuse1<<" secs! press any key go on..." << std::endl;
    #if AUTO_RUN
   // points = 500000000000;
    #else
    char temp_char;
	std::cin >> temp_char;
    #endif
    gettimeofday(&t3,NULL);
    std::stringstream ss;
    ss << points_per_dco;
    for (size_t i = 0; i < Dco_k; i++){
            ddo_ids[i] = UhconnApi::async(map_dcos[i], "montecarlo", ss.str());
    }
    DdoDataId mid_data_id[Dco_k];
    for (size_t i = 0; i < Dco_k; i++){
        UhconnDdo* ddo = UhconnApi::wait(ddo_ids[i]);
        DdoBlockData d;
        ddo->loadBlock(d);
        std::string str((const char*)d.data, d.len);
        std::stringstream tmpss(str);
        tmpss >> mid_data_id[i];
    }

    std::stringstream mid_stream;
    mid_stream << Dco_k;
    for (size_t i = 0; i < Dco_k; i++)
    {
        mid_stream << " "<< mid_data_id[i];
    }

    UhconnDdoRef tmpid = UhconnApi::async(reduce_dco, std::string("montecarlo"), mid_stream.str());
    UhconnDdo* ddo2 = UhconnApi::wait(tmpid);
    DdoBlockData d;
    ddo2->loadBlock(d);
    std::string pi((char*)(d.data));
   // double * pi = static_cast<double*>(d.data);
    gettimeofday(&t4,NULL);

    cout<<"dco create time = "<< timeuse1 <<"secs"<<endl;
    
    timeuse2 = (t4.tv_sec - t3.tv_sec) + (double)(t4.tv_usec - t3.tv_usec)/1000000.0;
    cout<<"calculate time = "<< timeuse2 <<" secs"<<" for "<<Dco_k<<" DCOs!"<<endl;

    cout<<"total time = "<< timeuse1 + timeuse2 <<"secs"<<endl;
	std::cout << "Approximated pi value: " << pi <<std::endl;

}
