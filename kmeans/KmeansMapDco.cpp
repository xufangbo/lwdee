#include "KmeansMapDco.h"
#include "kmeansMap.h"
#include <sstream>
#include "core/UhconnSimpleDB.h"
#include "core/UhconnDdoMgr.h"
#include "libgo.h"

std::string  KmeansMapDco::init(std::string a) {
    std::stringstream ss(a);
    DdoDataId did;
    DdoDataId meansID;
    int clusters_n;
    int dim;
    ss >> did >> meansID >> clusters_n >> dim;
    DdoBlockData data;
    UhconnSimpleDB::getInstance().loadBlock(did, data);
    DdoBlockData means;
    UhconnSimpleDB::getInstance().loadBlock(meansID, means);
    kmap = new KMeansMap(clusters_n,100,dim);
    kmap->pointsFromBuf(data.data, data.len, dim);
    kmap->meansFromBuf(means.data, means.len, dim);
    // kmap->printMeans();
    std::cout<<"KmeansMapDco::init OK!"<<std::endl;
    return "success";
};

std::string  KmeansMapDco::init1(std::string a) {
    std::stringstream ss(a);
    int fstart;
    int fnum;
    DdoDataId meansID;
    int clusters_n;
    int dim;
    ss >> fstart >> fnum >> meansID >> clusters_n >> dim;
    std::cout<<"KmeansMapDco::init1 file start: "<<fstart<<" fnum:"<<fnum<<" cluster:"<<clusters_n<<"dim: "<<dim<<std::endl;
    DdoBlockData means;
    UhconnSimpleDB::getInstance().loadBlock(meansID, means);
    kmap = new KMeansMap(clusters_n,100,dim);
    // kmap->pointsFromBuf(data.data, data.len, dim);
    auto points = kmap->getPoints();
    KMeans::loadPoints(fstart, fnum, points);
    
    kmap->meansFromBuf(means.data, means.len, dim);
    // kmap->printMeans();
    std::cout<<itsAddr()<<" KmeansMapDco::init OK! points_.size = "<<points->size()
    <<", means.size= "<<kmap->get_means().size()<<std::endl;
    return "success";
};

std::string  KmeansMapDco::init2(std::string a) {
    std::stringstream ss(a);
    int fstart;
    int fnum;
    DdoDataId meansID;
    int clusters_n;
    int dim;
    ss >> fstart >> fnum >> meansID >> clusters_n >> dim;
    std::cout<<"KmeansMapDco::init2 file start: "<<fstart<<" fnum:"<<fnum<<" cluster:"<<clusters_n<<"dim: "<<dim<<std::endl;
    
    //UhconnSimpleDB::getInstance().loadBlock(meansID, means);
    DdoBlockData * means = UhconnSimpleDB::getInstance().getBlock(meansID);
    kmap = new KMeansMap(clusters_n,100,dim);
    // kmap->pointsFromBuf(data.data, data.len, dim);
    auto points = kmap->getPoints();
    KMeans::loadPoints(fstart, fnum, points);
    
    kmap->meansFromBuf(means->data, means->len, dim);
    // kmap->printMeans();
    // std::cout<<itsAddr()<<" KmeansMapDco::init OK! points_.size = "<<points->size()
    // <<", means.size= "<<kmap->get_means().size()<<std::endl;
    return "success";
};

std::string KmeansMapDco::runOneStep(std::string a) {
    std::cout <<itsAddr()<<"runOneStep"<<std::endl;
    bool re = kmap->runOneStep();
    DdoDataId did= UhconnSimpleDB::getInstance().genDataId();
    std::cout<<"runOneStep^^ genDataId:"<<did<<std::endl;
    DdoBlockData d;
    d.len = kmap->resultLen();
    d.data = malloc(d.len);
    kmap->resultToBuf(d.data, d.len);
    // std::cout << "ddo_id(m):" <<did <<" data_len:"<<d.len<<std::endl;
    UhconnSimpleDB::getInstance().storeBlock(did, d);
    free(d.data);
    std::stringstream ss;
    ss << re << " " << did;
    return ss.str();
};

#ifdef USE_THREAD_CH
typedef struct run_param_t{
    bool ret;
    KMeansMap * kmap;
	co::co_chan<int> ch;
}run_param;

void* wrapper_runOneStep(void* param){
   run_param *p = (run_param *)param;
   p->ret = p->kmap->runOneStep();
   (p->ch) << 2;
}
#endif //#ifdef USE_THREAD_CH

std::string KmeansMapDco::runOneStep2(std::string a) {
    std::cout <<itsAddr()<<"runOneStep"<<std::endl;
    std::stringstream bss(a);
    bool re; 
    int reduce_node;
    bss >> reduce_node;
    #ifdef SEP_THREAD
    void* proc = co_sched.GetCurrentProc();
	if(proc){
        co_sched.detachSched(proc);
	}else{
		std::cout<<"obtain proc failed"<<std::endl;
	}
	#endif //#if SEP_THREAD
    #ifdef USE_THREAD_CH
    run_param param;
    param.kmap = kmap;
	int tmp;
    pthread_t threads;
    struct sched_param sp;
    bzero((void*)&sp, sizeof(sp));
    sp.sched_priority = sched_get_priority_max(SCHED_FIFO);

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	pthread_attr_setschedparam(&attr, &sp);
    std::cout << "create new thread.."<<std::endl;
	pthread_create(&threads, &attr, wrapper_runOneStep, (void*)&param);
	pthread_detach(threads);
	param.ch >> tmp;
    re = param.ret;
    #else
    re = kmap->runOneStep();
    #endif//#ifdef USE_THREAD_CH
    DdoDataId did= UhconnSimpleDB::getInstance().genDataId();
    unsigned int len = kmap->resultLen();
    DdoBlockData *d =  UhconnSimpleDB::getInstance().createBlock(did,len,1);
    kmap->resultToBuf(d->data, d->len);
    // std::cout << "ddo_id(m):" <<did <<" data_len:"<<d.len<<std::endl;
    UhconnDdoMgr::getInstance().pushDdoToRemote(reduce_node, did);
    std::stringstream ss;
    ss << re << " " << did;
    #ifdef SEP_THREAD
	if(proc)
	{
        co_sched.attachSched(proc);
	}
	#endif //#if SEP_THREAD
    return ss.str();
};

std::string KmeansMapDco::set_means(std::string a) {
    std::stringstream ss(a);
    DdoDataId did;
    ss >> did;
    DdoBlockData md;
    UhconnSimpleDB::getInstance().loadBlock(did, md);
    // std::cout << "set_means-md.len:"<<md.len<<" did:"<<did<<std::endl;
    kmap->meansFromBuf(md.data, md.len, kmap->itsDim());
    // std::cout<<"---------------------means begin(m)-----------------------"<<std::endl;
    // kmap->printMeans();
    // std::cout<<"---------------------means end(m)-----------------------"<<std::endl;
    return "ok";
};


std::string KmeansMapDco::set_means2(std::string a) {
    std::stringstream ss(a);
    DdoDataId did;
    ss >> did;
    #ifdef SEP_THREAD
    void* proc = co_sched.GetCurrentProc();
	if(proc){
        co_sched.detachSched(proc);
	}else{
		std::cout<<"obtain proc failed"<<std::endl;
	}
	#endif //#if SEP_THREAD
    DdoBlockData *md =  UhconnSimpleDB::getInstance().getBlock(did);
    //UhconnSimpleDB::getInstance().loadBlock(did, md);
    // std::cout << "set_means-md.len:"<<md.len<<" did:"<<did<<std::endl;
    kmap->meansFromBuf(md->data, md->len, kmap->itsDim());
    // std::cout<<"---------------------means begin(m)-----------------------"<<std::endl;
    // kmap->printMeans();
    // std::cout<<"---------------------means end(m)-----------------------"<<std::endl;
    #ifdef SEP_THREAD
	if(proc)
	{
        co_sched.attachSched(proc);
	}
	#endif //#if SEP_THREAD
    return "ok";
};


KmeansMapDco::KmeansMapDco()
{
    getFunctionTable()["init"] = (PTR)&KmeansMapDco::init;
    getFunctionTable()["init1"] = (PTR)&KmeansMapDco::init1;    
    getFunctionTable()["init2"] = (PTR)&KmeansMapDco::init2;  
    getFunctionTable()["runOneStep"] =  (PTR)&KmeansMapDco::runOneStep;
    getFunctionTable()["runOneStep2"] =  (PTR)&KmeansMapDco::runOneStep2;
    getFunctionTable()["setMeans"] =  (PTR)&KmeansMapDco::set_means;
    getFunctionTable()["setMeans2"] =  (PTR)&KmeansMapDco::set_means2;
}

KmeansMapDco::~KmeansMapDco()
{

}