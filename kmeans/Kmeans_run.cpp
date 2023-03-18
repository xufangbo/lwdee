
#include "UserDco.h"
#include "api/UhconnApi.h"
#include "api/UhconnDcoRef.h"
#include "core/UhconnRouter.h"
#include "core/UhconnWorkNode.h"
#include <iostream>
#include <string>
#include <libgo.h>
#include <cstdlib>
#include "api/UhconnDdo.h"
#include "api/UhconnDdoRef.h"
#include "core/UhconnSimpleDB.h"

#ifdef RUN_DEMO
#include "core/UhconnConfig.h"
#include "core/UhconnVoxorFactory.h"
#include "core/UhconnRouter.h"
#include "UserDcoFactory.h"
#include <map>
#endif   
#include <vector>
#include "point.h"
#include "kmeans.h"
#include "kmeansMap.h"
#include "kmeansReduce.h"
#include <sys/time.h>
#include <random>
#include <shell_cpp.h>
#include "Benchmark.h"

using namespace std;

static void usage() {
  #ifndef RUN_DEMO
  cout << "Usage: ./kmeans <filename> <k> <n>" << endl;
  cout << "   file should be space-separated points, one point per line."
  << endl;
  cout << "   k is the number of desired clusters."
  << endl;
  cout << "   n is the number of desired dco."
  << endl;
  #else
  cout << "kmeans dataPath cluster dcoCount nodeCount file_count nodeConfFile nodeName" << endl;
  #endif
  return;
}

template<typename T>
std::string to_str(T input) {
    std::stringstream ss;
    ss << input;
    return ss.str();
}

template<typename T>
std::string to_str(T input, std::string str) {
    std::stringstream ss(str);
    ss << " " << input;
    return ss.str();
}
#ifndef RUN_DEMO
int kmeans_run(int argc, char *argv[]) {
#else
int main0(int argc, char *argv[]) {
#endif    
    if (argc < 3) {
        usage();
        return -1;
    }
    const std::string fpath = argv[1];
    const int num_clusters = atoi(argv[2]);   
    size_t Dco_k =  atoi(argv[3]);  
 #ifdef RUN_DEMO
    const std::string confPath = argv[4];
    const std::string nodeName = argv[5];
    size_t nodeAmount =  atoi(argv[6]);

// #if 1
    if(UhconnConfig::getInstance().loadConf(confPath,nodeName) != 0){
        std::cout<<"load config failed!!"<<endl;
    }
    if(nodeAmount>0){
        UhconnConfig::getInstance().setNodeAmount(nodeAmount);
    }
    UhconnVoxorFactory::getInstance().setupLocalWorkEnvironment(new UserDcoFactory(),UhconnConfig::getInstance().getNodeId()); 
    UhconnWorkNode* workNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
    workNode->itsRouter().setupRouteInfoFromConf();

    // char testData[]={"Hello,data!!"};
    // DdoBlockData block;
    // block.type = 0;
    // block.len = sizeof(testData);
    // std::cout<<"test str len = "<< sizeof(testData)<<endl;
    // block.data= testData;
    // DdoDataId ddoId = 100;
    // DdoDataId nddoId = 101;
    // UhconnSimpleDB::getInstance().storeBlock(ddoId,block);
    // workNode->itsRouter().pullData(workNode->itId(),ddoId);
    // DdoBlockData block1;
    // int ret = UhconnSimpleDB::getInstance().loadBlock(nddoId,block1);
    // if(ret < 0){
    //     std::cout << "load data failed!" << std::endl;
    // }else{
    //     std::cout<<"pull data len: "<<block1.len<<" value:"<<std::string((char*)block1.data)<<std::endl;
    // }

    // UhconnMessage testMsg(workNode->itId(),workNode->itId(),MSG_CMD_CREATE,MSG_TYPE_REQ);
    // testMsg.setClassName("testClass");
    // testMsg.setDdoId(100);
    // testMsg.setMethodPara(std::string("tets-param"));
    // testMsg.setMsgId(66);
    // workNode->itsRouter().sendMsg(testMsg);

   if(nodeName.compare("node1")){
    while(1){
        usleep(10000);
    }
   }

#endif     
    Benchmark("Total:");
    // 输入参数整理
    //读所有原始数据
    std::vector<std::vector<std::pair<Point,int>>> mid_val;
    std::vector<Point> points;
    KMeans::loadPoints(fpath, &points);
    int dim = points[0].dim();
    struct timeval t1,t2;
    double timeuse;
 
  //初始化质心
    std::random_device rd;
    std::mt19937 rng(rd());
    uniform_int_distribution<long> dist(0,points.size()-1);
    cout<<"num_clusters:"<<num_clusters<<"points:"<<points.size()<<" rand:"<<endl;
    std::vector<Point> origin_means;
    std::vector<long> randgen;
    long randIndex[] = {8756, 8869, 4792, 8329, 9316,
                        7807, 463,  7530, 5026, 4683};
    if (dim != 784) {
      for (int i = 0; i < num_clusters; i++) {
        long rand = dist(rng);
        randgen.push_back(rand);
        cout << rand << " ";
        origin_means.push_back(points[rand]);
      }
    } else {

      for (auto it : randIndex) {
        origin_means.push_back(points[it]);
      }
    }
    //原始数据分K组
    std::vector<Point>* array = new std::vector<Point>[Dco_k];

    for (size_t i = 0; i < points.size(); i++)
    {
        array[i % Dco_k].push_back(points[i]);
    }
    //开始计时

    std::cout << "step 1 create" << std::endl;
    UhconnDcoRef* kmaps = new UhconnDcoRef[Dco_k];
    std::vector<UhconnDdoRef> ddors(Dco_k);

    DdoDataId means_did= UhconnSimpleDB::getInstance().genDataId();
    DdoBlockData means_data;
    means_data.len = KMeans::size(origin_means);
    means_data.data = malloc(means_data.len);
    KMeans::toBuf(origin_means, means_data.data, means_data.len);
    UhconnSimpleDB::getInstance().storeBlock(means_did, means_data);
    if(means_data.data ) {
        free(means_data.data);
    }
   //create dco(s)
    for (size_t i = 0; i < Dco_k; i++)
    {
        kmaps[i] = UhconnApi::create(std::string("KmeansMapDco"));
        //初始化 
        DdoDataId did= UhconnSimpleDB::getInstance().genDataId();
        DdoBlockData d;
        d.len = KMeans::size(array[i]);
        d.data = malloc(d.len);
        KMeans::toBuf(array[i], d.data, d.len);
        UhconnSimpleDB::getInstance().storeBlock(did, d);
        if( d.data ) {
            free(d.data);
        }
        std::stringstream ss;
        ss << did << " " << means_did << " " << num_clusters << " " << dim;
        ddors[i] = UhconnApi::async(kmaps[i], std::string("init"), ss.str());
    }

    UhconnDcoRef krds = UhconnApi::create(std::string("KmeansReduceDco"));
    std::stringstream tmp;
    tmp << num_clusters << " " << dim;
    UhconnDdoRef tmpid = UhconnApi::async(krds, std::string("init"), tmp.str());
    std::cout <<"start to wait init result"<<std::endl; 
    for (size_t i = 0; i < Dco_k; i++)
    {
        std::cout <<"wait "<<i<<" start!"<<std::endl; 
        UhconnApi::wait(ddors[i]);
        std::cout <<"wait "<<i<<" end!"<<std::endl; 
    }
    UhconnApi::wait(tmpid);

    std::cout << "step 2 loop" << std::endl;
    int loopCount = 0;
    gettimeofday(&t1,NULL);
    while(true) {
        std::cout << "***********************iterate:" <<loopCount<<"*********************"<< std::endl;
        for (size_t i = 0; i < Dco_k; i++)
        {
            ddors[i] = UhconnApi::async(kmaps[i], "runOneStep", "");
        }
        bool result = true;
        DdoDataId mid_data_id[Dco_k];

        for (size_t i = 0; i < Dco_k; i++)
        {
            UhconnDdo* ddo = UhconnApi::wait(ddors[i]);
            DdoBlockData d;
            ddo->loadBlock(d);
            std::string str((const char*)d.data, d.len);
            std::stringstream tmpss(str);
            bool rst;
            tmpss >> rst >> mid_data_id[i];

            result &= rst;
        }
        if(result) {
            break;
        }

        std::stringstream mid_stream;
        mid_stream << Dco_k;
        for (size_t i = 0; i < Dco_k; i++)
        {
            mid_stream << " "<< mid_data_id[i];
        }
    
        tmpid = UhconnApi::async(krds, std::string("measFromStepResult"), mid_stream.str());
        UhconnDdo* ddo2 = UhconnApi::wait(tmpid);
        DdoBlockData d;
        ddo2->loadBlock(d);
        std::string newmeans_str((const char*)d.data, d.len);
        for (size_t i = 0; i < Dco_k; i++)
        {
            ddors[i] = UhconnApi::async(kmaps[i], "setMeans", newmeans_str);
            UhconnDdo* ddo = UhconnApi::wait(ddors[i]);
            ddo = ddo;
        }     
        loopCount++;   
    }

    std::cout << "iterate count:" <<loopCount<< std::endl;
    gettimeofday(&t2,NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    cout<<"time = "<<timeuse<<"secs"<<endl;


    std::cout << "origin_means_index: " << std::endl; 
    if(dim != 784 ) {
        for(auto it : randgen){
            std::cout << it <<", ";
        }
    }
    else {
        for(auto it : randIndex){
            std::cout << it <<", ";
        }

    }
    std::cout << std::endl;
    return 0;
}

#ifndef RUN_DEMO

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), kmeansRun, kmeans_run, kmeans test);

#endif


#define DIM_FIX 62720
#define TOTAL_FILES 120
// #define NUM_CLUSTER 1000


#ifndef RUN_DEMO
int kmeans_run1(int argc, char *argv[]) {
#else
int main1(int argc, char *argv[]) {
#endif    
    if (argc < 3) {
        usage();
        return -1;
    }

    const std::string fpath = argv[1];
    const int num_clusters = atoi(argv[2]);
    size_t Dco_k =  atoi(argv[3]);

#ifdef RUN_DEMO
    size_t nodeAmount =  atoi(argv[4]);
    const std::string confPath = argv[5];
    const std::string nodeName = argv[6];
    if(UhconnConfig::getInstance().loadConf(confPath,nodeName) != 0){
        std::cout<<"load config failed!!"<<endl;
    }
    if(nodeAmount>0){
        UhconnConfig::getInstance().setNodeAmount(nodeAmount);
    }
    UhconnVoxorFactory::getInstance().setupLocalWorkEnvironment(new UserDcoFactory(),UhconnConfig::getInstance().getNodeId()); 
    UhconnWorkNode* workNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
    workNode->itsRouter().setupRouteInfoFromConf();
   if(nodeName.compare("node1")){
    while(1){
        usleep(10000);
    }
   }

#endif     
 
    // 分配文件
    typedef struct file_depart {
        int start;
        int num;
    }file_info;
    file_info* aaa = new file_info[Dco_k];
    for (size_t i = 0; i < Dco_k; i++)
    {
        aaa[i].num = TOTAL_FILES/Dco_k;
    }
    for (size_t i = 0; i < TOTAL_FILES%Dco_k; i++)
    {
        aaa[i].num++;
    }
   
    // uint16_t totalnum = 0;
    assert( Dco_k > 0 );
    aaa[0].start = 0;

    for (size_t i = 1; i < Dco_k; i++)
    {
        aaa[i].start = aaa[i-1].start + aaa[i-1].num;

    }
   std::cout <<"file distribute:"<<std::endl;
    for(size_t i=0;i<Dco_k;i++){
       std::cout<<"dco "<<i<<":"<<aaa[i].start<<"-"<<aaa[i].start+aaa[i].num<<"("<<aaa[i].num<<")"<<std::endl;
    }
    //读所有原始数据
    // std::vector<std::vector<std::pair<Point,int>>> mid_val;
    // std::vector<Point> points;
    // KMeans::loadPoints(fpath, &points);
    int dim = DIM_FIX;//points[0].dim();
    struct timeval t1,t2;
    double timeuse;
 
  //初始化质心
    std::vector<Point> origin_means;
    KMeans::loadPoints(0, 3, &origin_means);
    std::cout<<"load origin means done, total "<<origin_means.size()<<" pionts, dim="<<origin_means[0].dim()<<std::endl;
    std::cout << "step 1 create dcos" << std::endl;
    UhconnDcoRef* kmaps = new UhconnDcoRef[Dco_k];
    std::vector<UhconnDdoRef> ddors(Dco_k);

    DdoDataId means_did= UhconnSimpleDB::getInstance().genDataId();
    DdoBlockData means_data;
    means_data.len = KMeans::size(origin_means);
    means_data.data = malloc(means_data.len);
    KMeans::toBuf(origin_means, means_data.data, means_data.len);
    UhconnSimpleDB::getInstance().storeBlock(means_did, means_data);
    if(means_data.data ) {
        free(means_data.data);
    }
   //create dco(s)
    for (size_t i = 0; i < Dco_k; i++)
    {
        kmaps[i] = UhconnApi::create(std::string("KmeansMapDco"));
        std::stringstream ss;
        ss << aaa[i].start << " " << aaa[i].num << " " << means_did << " " << num_clusters << " " << dim;
        ddors[i] = UhconnApi::async(kmaps[i], std::string("init1"), ss.str());
        std::cout<< ss.str() << std::endl;
    }

    UhconnDcoRef krds = UhconnApi::create(std::string("KmeansReduceDco"));
    std::stringstream tmp;
    tmp << num_clusters << " " << dim;
    UhconnDdoRef tmpid = UhconnApi::async(krds, std::string("init"), tmp.str());
    std::cout <<"start to wait init result"<<std::endl; 
    for (size_t i = 0; i < Dco_k; i++)
    {
        std::cout <<"wait "<<i<<" start!"<<std::endl; 
        UhconnApi::wait(ddors[i]);
        std::cout <<"wait "<<i<<" end!"<<std::endl; 
    }
    // UhconnSimpleDB::getInstance().deleteBlock(means_did);
    UhconnApi::wait(tmpid);

    std::cout << "step 2 loop" << std::endl;
    int loopCount = 0;

        //开始计时
    gettimeofday(&t1,NULL);
    while(true && loopCount < 30) {
        std::cout << "***********************iterate:" <<loopCount<<"*********************"<< std::endl;
        for (size_t i = 0; i < Dco_k; i++)
        {
            ddors[i] = UhconnApi::async(kmaps[i], "runOneStep", "");
        }
        bool result = true;
        DdoDataId mid_data_id[Dco_k];

        for (size_t i = 0; i < Dco_k; i++)
        {
            UhconnDdo* ddo = UhconnApi::wait(ddors[i]);
            DdoBlockData d;
            ddo->loadBlock(d);
            std::string str((const char*)d.data, d.len);
            std::stringstream tmpss(str);
            bool rst;
            tmpss >> rst >> mid_data_id[i];

            result &= rst;
            delete ddo;
        }
        if(result) {
            break;
        }

        std::stringstream mid_stream;
        mid_stream << Dco_k;
        for (size_t i = 0; i < Dco_k; i++)
        {
            mid_stream << " "<< mid_data_id[i];
        }
    
        tmpid = UhconnApi::async(krds, std::string("measFromStepResult"), mid_stream.str());
        UhconnDdo* ddo2 = UhconnApi::wait(tmpid);
        DdoBlockData d;
        ddo2->loadBlock(d);
        delete ddo2;
        std::string newmeans_str((const char*)d.data, d.len);
        for (size_t i = 0; i < Dco_k; i++)
        {
            ddors[i] = UhconnApi::async(kmaps[i], "setMeans", newmeans_str);
            UhconnDdo* ddo = UhconnApi::wait(ddors[i]);
            ddo = ddo;
        }     

        loopCount++;   
    }

    std::cout << "iterate count:" <<loopCount<< std::endl;
    gettimeofday(&t2,NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    cout<<"time = "<<timeuse<<"secs"<<endl;

    return 0;
}



#ifndef RUN_DEMO
int kmeans_run2(int argc, char *argv[]) {
#else
int main(int argc, char *argv[]) {
#endif    
    if (argc < 3) {
        usage();
        return -1;
    }

    const std::string fpath = argv[1];
    const int num_clusters = atoi(argv[2]);
    size_t Dco_k =  atoi(argv[3]);

#ifdef RUN_DEMO
    size_t nodeAmount =  atoi(argv[4]);
    int total_file_cnt = atoi(argv[5]);
    const std::string confPath = argv[6];
    const std::string nodeName = argv[7];
    
    if(UhconnConfig::getInstance().loadConf(confPath,nodeName) != 0){
        std::cout<<"load config failed!!"<<endl;
    }
    if(nodeAmount>0){
        UhconnConfig::getInstance().setNodeAmount(nodeAmount);
    }
    UhconnVoxorFactory::getInstance().setupLocalWorkEnvironment(new UserDcoFactory(),UhconnConfig::getInstance().getNodeId()); 
    UhconnWorkNode* workNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
    workNode->itsRouter().setupRouteInfoFromConf();
   if(nodeName.compare("node1")){
    while(1){
        usleep(10000);
    }
   }

#endif     
 
    // 分配文件
    typedef struct file_depart {
        int start;
        int num;
    }file_info;
    file_info* aaa = new file_info[Dco_k];
    #ifdef RUN_DEMO
    for (size_t i = 0; i < Dco_k; i++)
    {
        aaa[i].num = total_file_cnt/Dco_k;
    }
    for (size_t i = 0; i < total_file_cnt%Dco_k; i++)
    {
        aaa[i].num++;
    }
    #else
    for (size_t i = 0; i < Dco_k; i++)
    {
        aaa[i].num = TOTAL_FILES/Dco_k;
    }
    for (size_t i = 0; i < TOTAL_FILES%Dco_k; i++)
    {
        aaa[i].num++;
    }
    #endif //RUN_DEMO
    // uint16_t totalnum = 0;
    assert( Dco_k > 0 );
    aaa[0].start = 0;
    for (size_t i = 1; i < Dco_k; i++)
    {
        aaa[i].start = aaa[i-1].start + aaa[i-1].num;
    }
   std::cout <<"file distribute:"<<std::endl;
    for(size_t i=0;i<Dco_k;i++){
       std::cout<<"dco "<<i<<":"<<aaa[i].start<<"-"<<aaa[i].start+aaa[i].num<<"("<<aaa[i].num<<")"<<std::endl;
    }

    int dim = DIM_FIX;//points[0].dim();
    struct timeval t1,t2,ts,te;
    double timeuse;
 
  //初始化质心
    std::vector<Point> origin_means;
    KMeans::loadPoints(0, 3, &origin_means);
    std::cout<<"load origin means done, total "<<origin_means.size()<<" pionts, dim="<<origin_means[0].dim()<<std::endl;
    std::cout << "step 1 create dcos" << std::endl;
    UhconnDcoRef* kmaps = new UhconnDcoRef[Dco_k];
    std::vector<UhconnDdoRef> ddors(Dco_k);

    DdoDataId means_did= UhconnSimpleDB::getInstance().genDataId();
    DdoBlockData * means_data;
    unsigned int means_len = KMeans::size(origin_means);
    means_data = UhconnSimpleDB::getInstance().createBlock(means_did, means_len,0);
    KMeans::toBuf(origin_means, means_data->data, means_data->len);
    
   //create dco(s)
    for (size_t i = 0; i < Dco_k; i++)
    {
        kmaps[i] = UhconnApi::create(std::string("KmeansMapDco"));
        std::stringstream ss;
        ss << aaa[i].start << " " << aaa[i].num << " " << means_did << " " << num_clusters << " " << dim;
        ddors[i] = UhconnApi::async(kmaps[i], std::string("init2"), ss.str());
        std::cout<< ss.str() << std::endl;
    }

    UhconnDcoRef krds = UhconnApi::create(std::string("KmeansReduceDco"));
    std::stringstream tmp;
    tmp << num_clusters << " " << dim;
    UhconnDdoRef tmpid = UhconnApi::async(krds, std::string("init"), tmp.str());
    std::cout <<"start to wait init result"<<std::endl; 
    for (size_t i = 0; i < Dco_k; i++)
    {
        std::cout <<"wait "<<i<<" start!"<<std::endl; 
        UhconnApi::wait(ddors[i]);
        std::cout <<"wait "<<i<<" end!"<<std::endl; 
    }
    // UhconnSimpleDB::getInstance().deleteBlock(means_did);
    UhconnApi::wait(tmpid);
    UhconnSimpleDB::getInstance().deleteBlockGlobal(means_did);
    std::cout << "step 2 loop" << std::endl;
    int loopCount = 0;

        //开始计时
    gettimeofday(&t1,NULL);
    while(true && loopCount < 3) {
        std::cout << "***********************iterate:" <<loopCount<<"*********************"<< std::endl;
        gettimeofday(&ts,NULL);
        int reduce_node = krds.itsBelongWorkNodeId();
        std::stringstream rdsIds;
        rdsIds<<reduce_node;
        for (size_t i = 0; i < Dco_k; i++)
        {
            ddors[i] = UhconnApi::async(kmaps[i], "runOneStep2", rdsIds.str());
        }
        bool result = true;
        DdoDataId mid_data_id[Dco_k];

        std::vector<UhconnDdo*> ddo = UhconnApi::wait(ddors,true);
        for (size_t i = 0; i < Dco_k; i++)
        {
            //UhconnDdo* ddo = UhconnApi::wait(ddors[i]);
            DdoBlockData *d = ddo[i]->getBlock();
            std::string str((const char*)d->data, d->len);
            std::stringstream tmpss(str);
            bool rst;
            tmpss >> rst >> mid_data_id[i];

            result &= rst;
            ddo[i]->deleteBlock();
            delete ddo[i];
        }
        if(result) {
            break;
        }

        std::stringstream mid_stream;
        mid_stream << Dco_k;
        for (size_t i = 0; i < Dco_k; i++)
        {
            mid_stream << " "<< mid_data_id[i];
        }
    
        tmpid = UhconnApi::async(krds, std::string("measFromStepResult2"), mid_stream.str());
        UhconnDdo* ddo2 = UhconnApi::wait(tmpid);
        DdoBlockData* d =ddo2->getBlock();
        //ddo2->loadBlock(d);
        std::string newmeans_str((const char*)d->data, d->len);

        for (size_t i = 0; i < Dco_k; i++)
        {
            ddors[i] = UhconnApi::async(kmaps[i], "setMeans2", newmeans_str);
            UhconnApi::wait(ddors[i]);
        } 
        UhconnSimpleDB::getInstance().deleteBlockGlobal(ddo2->itsRef().itsId());   
        ddo2->deleteBlock();
        delete ddo2;
        loopCount++;   
        gettimeofday(&te,NULL);
        timeuse = (te.tv_sec - ts.tv_sec) + (double)(te.tv_usec - ts.tv_usec)/1000000.0;
        cout<<"this iterate cost "<<timeuse<<" secs"<<endl;
    }

    std::cout << "iterate count:" <<loopCount<< std::endl;
    gettimeofday(&t2,NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    cout<<"time = "<<timeuse<<"secs"<<endl;

    return 0;
}

#ifndef RUN_DEMO

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), kmeansRun2, kmeans_run2, kmeans test);

#endif