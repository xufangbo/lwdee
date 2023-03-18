#include "KmeansReduceDco.h"
#include "kmeansReduce.h"
#include <sstream>
#include "kmeansReduce.h"
#include "core/UhconnSimpleDB.h"
#include "libgo.h"
#include <sys/time.h>

KMeansReduce* kreduce = NULL;

std::string  KmeansReduceDco::init(std::string a) {
    std::stringstream ss(a);
    int num_clusters;
    int dim;
    ss >> num_clusters >> dim;
    kreduce = new KMeansReduce(num_clusters);
    kreduce->init_mr(num_clusters,dim);
    return a;
};

std::string KmeansReduceDco::measFromStepResult(std::string a) {

    std::stringstream ss(a);
    size_t k;
    ss >> k;
    std::vector<DdoDataId> mid_data_id(k);
    // std::cout << "k=" <<k <<std::endl;
    for (size_t i = 0; i < k; i++)
    {
        ss >> mid_data_id[i];
        DdoBlockData block;
        UhconnSimpleDB::getInstance().loadBlock(mid_data_id[i], block);
        // std::cout << "ddo_id(r):" <<mid_data_id[i] <<" data_len:"<<block.len<<std::endl;
        kreduce->resultFromBuf(block.data, block.len);
    }
    kreduce->update_means();
    // std::cout <<"*******************means begin*********************"<<std::endl;
    // kreduce->printMeans();
   
    DdoDataId did= UhconnSimpleDB::getInstance().genDataId();
    DdoBlockData d;
    d.len = kreduce->meansLen();
    d.data = malloc(d.len);
    // std::cout<<"readuce-means id:"<<did<<" d.len"<<d.len<<std::endl;
    // std::cout <<"*******************means end*********************"<<std::endl;
    kreduce->meansToBuf(d.data, d.len);
    UhconnSimpleDB::getInstance().storeBlock(did, d); //@todo delete reduce ddo
    free(d.data);
    std::stringstream asss;
    asss << did;
    return asss.str();
};

#ifdef USE_THREAD_CH
typedef struct run_param_t{
	co::co_chan<int> ch;
}run_param_r;
run_param_r param_r;
void* wrapper_updateMeans(void* param){
   //run_param *p = (run_param *)param;
   kreduce->update_means();
   param_r.ch << 2;
}
#endif //#ifdef USE_THREAD_CH

std::string KmeansReduceDco::measFromStepResult2(std::string a) {

    std::stringstream ss(a);
    size_t k;
    ss >> k;
    std::vector<DdoDataId> mid_data_id(k);
    // std::cout << "k=" <<k <<std::endl;
    #ifdef SEP_THREAD
    void* proc = co_sched.GetCurrentProc();
	if(proc){
        co_sched.detachSched(proc);
	}else{
		std::cout<<"obtain proc failed"<<std::endl;
	}
	#endif //#if SEP_THREAD
    struct timeval t1,t2;
    double timeuse;
    gettimeofday(&t1,NULL);
    for (size_t i = 0; i < k; i++)
    {
        ss >> mid_data_id[i];
        DdoBlockData * block = UhconnSimpleDB::getInstance().getBlock(mid_data_id[i]);
        kreduce->resultFromBuf(block->data, block->len);
        UhconnSimpleDB::getInstance().deleteBlock(mid_data_id[i]);
    }
    gettimeofday(&t2,NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    std::cout<<"reduce pull mid data cost time = "<<timeuse<<"secs"<<std::endl;

    // #ifdef USE_THREAD_CH
	// int tmp;
    // pthread_t threads;
    // struct sched_param sp;
    // bzero((void*)&sp, sizeof(sp));
    // sp.sched_priority = sched_get_priority_max(SCHED_FIFO);

	// pthread_attr_t attr;
	// pthread_attr_init(&attr);
	// pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	// pthread_attr_setschedparam(&attr, &sp);
	// pthread_create(&threads, &attr, wrapper_updateMeans, (void*)&param_r);
	// pthread_detach(threads);
	// param_r.ch >> tmp;
    // #else
    kreduce->update_means();
   // #endif //#ifdef USE_THREAD_CH
   
    // std::cout <<"*******************means begin*********************"<<std::endl;
    // kreduce->printMeans();
   
    DdoDataId did= UhconnSimpleDB::getInstance().genDataId();
   
    unsigned int len = kreduce->meansLen();
    //d.data = malloc(d.len);
     DdoBlockData * d = UhconnSimpleDB::getInstance().createBlock(did, len,0);
    // std::cout<<"readuce-means id:"<<did<<" d.len"<<d.len<<std::endl;
    // std::cout <<"*******************means end*********************"<<std::endl;
    kreduce->meansToBuf(d->data, d->len);
    //UhconnSimpleDB::getInstance().storeBlock(did, d); //@todo delete reduce ddo
    //free(d.data);
    std::stringstream asss;
    asss << did;
    #ifdef SEP_THREAD
	if(proc)
	{
        co_sched.attachSched(proc);
	}
	#endif //#if SEP_THREAD
    return asss.str();
};


KmeansReduceDco::KmeansReduceDco()
{
    getFunctionTable()["init"] = (PTR)&KmeansReduceDco::init;
    getFunctionTable()["measFromStepResult"] = (PTR)&KmeansReduceDco::measFromStepResult; 
    getFunctionTable()["measFromStepResult2"] = (PTR)&KmeansReduceDco::measFromStepResult2;     
}

KmeansReduceDco::~KmeansReduceDco()
{

}