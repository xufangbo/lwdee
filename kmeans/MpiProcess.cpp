#include "MpiProcess.h"
#include <fstream>
#include <sstream>
#include <string>
#include "api/UhconnDdo.h"
#include <math.h>
#include "Benchmark.h"

MpiProcess::MpiProcess()
{
    getFunctionTable()["configClusters"] = (PTR)&MpiProcess::configClusters;
    getFunctionTable()["process"] = (PTR)&MpiProcess::process;
    samples = nullptr;
}

MpiProcess::~MpiProcess()
{
    if( samples != nullptr ) {
        delete []samples;
    }

}

void MpiProcess::initClusterCenter(float* center, int K_, int D_) {
    std::ifstream in("/mnt/data/origmeans.txt");
    for (int i = 0; i < K_; i++)
    {
        for (int j = 0; j < D_; j++)
        {
            in >> *center;
            ++center;
        }
    }
    
}

std::string MpiProcess::configClusters(std::string a) {
    std::stringstream ss(a);
    std::cout << "configClusters: " << a << std::endl;
    int filestart, fileNum, cluster_n, dim_n;
    ss >> filestart >> fileNum >> cluster_n >> dim_n;
    K = cluster_n;
    D = dim_n;
    enum{ linesPerFile = 400};
    N = fileNum * linesPerFile;
    samples = new float[N*D]();
    float* data_p = samples;
    for (int i = 0; i < fileNum; i++)
    {
        int dcout = linesPerFile * D;
        std::string fname;
        if( i != 0 ) {
            fname = "/mnt/data/x" + std::to_string(i);
        }
        else{
            fname = "/mnt/data/x";
        }
        std::ifstream in(fname);
        assert(in.is_open());
        while (dcout--)
        {
            in >> *data_p;
            data_p++;
        }
    }
    
    return std::to_string(N);
}

static float getDistance(float avector[],float bvector[],int n)
{
    float sum=0.0;
    for(int i = 0; i < n; i++) {
        sum += (avector[i]-bvector[i])*(avector[i]-bvector[i]);
    }
    return sum;
}

void MpiProcess::add_vector(float avector[],float bvector[],int n) {
    for(int i = 0; i < n; i++) {
        avector[i] += bvector[i];
    }
}
//cluster_did
std::string MpiProcess::process(std::string a) {
    std::stringstream ss(a);

    DdoDataId clusterid;
    ss >> clusterid;
    std::cout << "MpiProcess::process: " << clusterid << std::endl;
    DdoBlockData* d = UhconnSimpleDB::getInstance().getBlock(clusterid);
    
    float* cluster_p = (float*)d->data;
    float* data_p = samples;

    UhconnDdoRef sum_ref(this->itsAddr());
    float* sum_p = (float*)UhconnSimpleDB::getInstance().createBlock(sum_ref.itsId(), K*D*sizeof(float))->data;
    UhconnDdoRef num_ref(this->itsAddr());
    uint32_t* num_p = (uint32_t*)UhconnSimpleDB::getInstance().createBlock(num_ref.itsId(), K*sizeof(uint32_t))->data;
    for (int i = 0; i < N; i++)
    {
        float min_dist = __FLT_MAX__;
        int min_label = 0;
        for (int j = 0; j < K; j++)
        {
            float curr_dist = getDistance(data_p+i*D, cluster_p+j*D, D);
            if( curr_dist < min_dist ) {
                min_dist = curr_dist;
                min_label = j;
            }
        }
        //
        add_vector(sum_p+min_label*D, data_p+i*D, D);
        num_p[min_label]++;
    }
    std::cout << "MpiProcess::process exec once" << std::endl;
    // UhconnSimpleDB::getInstance().deleteBlock(clusterid); //删除质心数据，以便下次可以正常更新。
    return std::to_string(sum_ref.itsId()) +" "+std::to_string(num_ref.itsId());
}
