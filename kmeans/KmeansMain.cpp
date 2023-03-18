#include "KmeansMain.h"
#include <shell_cpp.h>
#include "api/UhconnApi.h"
#include "api/UhconnDdo.h"
#include "MpiProcess.h"
#include <sstream>
#include "Benchmark.h"

KmeansMain::KmeansMain()
{
    getFunctionTable()["start"] = (PTR)&KmeansMain::start;
}

KmeansMain::~KmeansMain()
{

}

std::string KmeansMain::start(std::string a) {
    int cluster_n;
    int dco_n;
    int file_n;
    int dim;
    std::stringstream ss(a);
    ss  >> cluster_n 
        >> dco_n
        >> file_n
        >> dim;

    // 分配文件
    fgroup_info* fgroup = new fgroup_info[dco_n];
    splitFiles(fgroup, dco_n, file_n);

    // //生成初始质心ddo
    UhconnDdoRef df_cluster(this->itsAddr());
    UhconnDdo clusterDdo(df_cluster);
    DdoBlockData* clusterBlock = UhconnSimpleDB::getInstance().createBlock(df_cluster.itsId(), cluster_n*dim*sizeof(float));
    MpiProcess::initClusterCenter((float*)clusterBlock->data, cluster_n, dim);
    std::cout << "origCluster: " << df_cluster.itsId() << std::endl;

    auto tt = new Benchmark("Total: ");
    std::cout << "step 1 create & config processor" << std::endl;
    UhconnDcoRef* process_ref = new UhconnDcoRef[dco_n];
    std::vector<UhconnDdoRef> ddors(dco_n);
    for (int i = 0; i < dco_n; i++) {
        process_ref[i] = create("MpiProcess");
        std::stringstream para;
        para  << fgroup[i].start << " " 
            << fgroup[i].num << " " 
            << cluster_n << " " 
            << dim;
        ddors[i] = async(process_ref[i], "configClusters", para.str());
    }
    
    std::cout <<"start to wait init result"<<std::endl; 

    wait(ddors, false);


    std::cout << "step 2 loop" << std::endl;
    int loopCount = 0;
    DdoDataId clusterID = df_cluster.itsId();
    while( loopCount++ < 3) {
        std::cout << "***********************iterate:" <<loopCount<<"*********************"<< std::endl;
        for (int i = 0; i < dco_n; i++) {
            std::stringstream para;
            para << clusterID;
            ddors[i] = async(process_ref[i], "process", para.str());
        }
        std::vector<UhconnDdo*> mid_ddo = wait(ddors, true);

        // UhconnSimpleDB::getInstance().deleteBlock(clusterID);
        DdoDataId mid_sum_id[dco_n];
        DdoDataId mid_cnt_id[dco_n];

        for (int i = 0; i < dco_n; i++)
        {
            DdoBlockData *d = mid_ddo[i]->getBlock();
            std::string str((const char*)d->data, d->len);
            std::stringstream tmpss(str);
            tmpss >> mid_sum_id[i] >> mid_cnt_id[i];
            // delete mid_ddo[i];
        }
        std::cout << "process complete " << loopCount << std::endl;
        //计算新的质心
        UhconnDdoRef new_cluster(this->itsAddr());
        clusterID = new_cluster.itsId();
        DdoBlockData* newclusterBlock = UhconnSimpleDB::getInstance().createBlock(new_cluster.itsId(), cluster_n*dim*sizeof(float));
        float* pCluster = (float*)newclusterBlock->data;
        uint32_t mem[cluster_n] = {0};
        
        auto tm_pull = new Benchmark("Pull: ");

        std::vector<DdoBlockData*> pdblk(dco_n);
        std::vector<DdoBlockData*> pnblk(dco_n);
        for (int i = 0; i < dco_n; i++)
        {
            pdblk[i] = UhconnSimpleDB::getInstance().getBlock(mid_sum_id[i], process_ref[i].itsBelongWorkNodeId());
            pnblk[i] = UhconnSimpleDB::getInstance().getBlock(mid_cnt_id[i], process_ref[i].itsBelongWorkNodeId());
        }
        delete tm_pull;

        std::cout  << " all dco process result sum pulled " << loopCount << std::endl;

        auto tm_reduce = new Benchmark("Reduce: ");

        for (int n = 0; n < dco_n; n++)
        {
            // std::cout << "cluster " << n << " len: " << pdblk[n]->len/sizeof(float) << std::endl;
            // std::cout << "cluster " << n << " mem: " << pnblk[n]->len/sizeof(uint32_t) << std::endl;
            if( pdblk[n]->len == 0 || pnblk[n]->len == 0 ) {
                continue;
            }
            float* pdata = (float*)pdblk[n]->data;
            uint32_t* pcnt = (uint32_t*)pnblk[n]->data;
            for (int i = 0; i < cluster_n * dim; i++)
            {
                pCluster[i] += pdata[i];
            }
            for (int i = 0; i < cluster_n; i++)
            {
                mem[i] += pcnt[i];
            }
            UhconnSimpleDB::getInstance().deleteBlock(mid_sum_id[n]);
            UhconnSimpleDB::getInstance().deleteBlock(mid_cnt_id[n]);
        }
        std::cout  << " calculate sum complete " << loopCount << std::endl;
        for (int k = 0; k < cluster_n; k++)
        {
            for (int d = 0; d < dim; d++)
            {
                *pCluster /= mem[k];
                ++pCluster;
            }
            
        }
        delete tm_reduce;
    }
    delete tt;
    return "success";
}

void KmeansMain::splitFiles(fgroup_info* pif, int group_num, int topFiles) {
    for (int i = 0; i < group_num; i++)
    {
        pif[i].num = topFiles/group_num;
    }
    for (int i = 0; i < topFiles%group_num; i++)
    {
        pif[i].num++;
    }

    pif[0].start = 0;
    for (int i = 1; i < group_num; i++)
    {
        pif[i].start = pif[i-1].start + pif[i-1].num;
    }
    std::cout <<"file distribute:"<<std::endl;
    for(int i=0; i < group_num; i++){
       std::cout << "dco " << i <<":"<<pif[i].start<<"-"<<pif[i].start+pif[i].num<<"("<<pif[i].num<<")"<<std::endl;
    }
}

int testKmeans(int argc, char* argv[]) {
    if (argc < 3) {
        // usage();
        return -1;
    }
    const std::string fpath = argv[1];
    const int num_clusters = atoi(argv[2]);
    int Dco_k =  atoi(argv[3]);
    int total_files = 120;
    int dim = 62720;
    if( argc > 4) {
        total_files = atoi(argv[4]);
    }
    if( argc > 5) {
        dim = atoi(argv[5]);
    }


    UhconnDcoRef dco_km = UhconnApi::create("KmeansMain");
    std::stringstream ss;
    ss  << num_clusters << " " 
        << Dco_k << " "
        << total_files << " "
        << dim;
    UhconnDdoRef rf_km = UhconnApi::async(dco_km, std::string("start"), ss.str());
    UhconnDdo* ddo_km = UhconnApi::wait(rf_km);
    return ddo_km->getBlock()->len;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), testkmeans, testKmeans, kmeans test);
