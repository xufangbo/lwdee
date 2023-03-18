#ifndef MPIPROCESS_H
#define MPIPROCESS_H

#pragma once
#include "api/UhconnDco.h"

class MpiProcess : public UhconnDco
{
public:
    MpiProcess();
    ~MpiProcess();

    static void initClusterCenter(float* center, int K_, int D_);
    void add_vector(float avector[],float bvector[],int n);
private:
    std::string configClusters(std::string a);  //filestart, fileNum, cluster_n, dim_n
    std::string process(std::string a); //cluster_did

    int K, N, D; //聚类数量（cluser_n）， 样本量(sample_n)， 维度(dim_n)
    float* samples; //样本数据 [N][D]
    // int* cluster_label; //记录每个点所属聚类序号
    // float* cluster_center; //存储聚类中心点

};

#endif