#include <iostream>
#include <sstream>
#include <random>
#include <ctime>
#include <thread>
#include <cstdlib>
#include <chrono> 
#include <mutex>
#include <sys/time.h>
#include "montecarloMap.h"
#include "core/UhconnSimpleDB.h"

#include <libgo.h>

#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/sem.h>
#include <string.h>
#include <assert.h>


// void error_callback(int error, const char* description) {
// 	fprintf(stderr, "Error: %s\n", description);
// }
#define MONTECARLO_DEBUG 0

#define USE_PROC 0
#define USE_THREAD_JOIN 0
#define USE_THREAD_MTX 0
#define USE_THREAD_CH 0
#define USE_PTHREAD_CH 1
#define USE_CO  0
#define SEP_THREAD 0

#define RAND_ARRAY_SIZE 100000
#define USE_RAND 1

#if !USE_RAND
double rand_array[RAND_ARRAY_SIZE]={0};
int  is_rand_array_init = 0;
void gen_rand(){
	is_rand_array_init = 1;
	for(unsigned int i=0;i<RAND_ARRAY_SIZE;i++){
		rand_array[i]= rand()/(RAND_MAX+1.0);
	}
	std::cout<<"rand init done!!"<<std::endl;
}
#endif

#if USE_PROC
int getSharedMem(char * name, int index, int size, int * shmid, void** pshm){
	int id;
	char *shmptr;
	key_t key;
	if((key = ftok(name, index)) < 0){
		printf("ftok error:%s\n", strerror(errno));
		return -1;
	}
	if((id = shmget(key, size, 0600|IPC_CREAT)) < 0){
		printf("shmget error:%s\n", strerror(errno));
		return -2;
	}
	if((shmptr = (char*)shmat(id, 0, 0)) == (void*)-1){
		printf("shmat error:%s\n", strerror(errno));
		return -3;
	}
    *pshm = shmptr;
	*shmid = id;
	return 0;
}

int rmShareMem(int shmid){
    if((shmctl(shmid, IPC_RMID, 0) < 0)){
		printf("shmctl error:%s\n", strerror(errno));
		return -1;
    }
	return 0;
}

union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
};

int getSem(char * name, int index, int initValue, int * semid)
{
	int id;
	key_t key;
	if((key = ftok(name, index)) < 0){
		printf("ftok error:%s\n", strerror(errno));
		return -1;
	}
    id = semget(key, 1, 0664);
    if(id == -1)
    {
        id = semget(key, 1, 0664 | IPC_CREAT);
        assert(id != -1);
        union semun v;
        v.val = initValue;
        semctl(id, 1, SETVAL, v);        
    }
	*semid = id;
	return 0;
}

int waitSem(int semid){
	struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = SEM_UNDO;
    return  semop(semid, &op, 1);
}

int postSem(int semid){
	struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = SEM_UNDO;
    return semop(semid, &op, 1);
}

int rmSem(int semid){
	return semctl(semid, 0, IPC_RMID, NULL);
}
#endif //#if USE_PROC

#if 0
int montecarloMap::montecarlo(unsigned long long &points,
                                 unsigned long long &count_in,
								 unsigned long long &count_out) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<double> dist(-1.0, 1.0);

	// unsigned long long count_in = 0;
	// unsigned long long count_out = 0;
	unsigned long long count_total = 0;

	while (count_total < points) {
		double rx = dist(rng);
		double ry = dist(rng);
		if (pow(rx, 2) + pow(ry, 2) <= 1) {
			count_in++;
		}
		else {
			count_out++;
		}

		count_total++;
        // if(count_total>0 && count_total%10000000==0){
        //             std::cout << count_total << std::endl;
        // }
    }

	//return (double)count_in / (double)(count_out + count_in) * 4;
	return 0;
}
#else

int montecarlo(unsigned long long &points,
			unsigned long long &count_in) {

	unsigned long long count_total = 0;
	double rx,ry;
    srand((int)time(0));

	struct timeval t1,t2;
    double timeuse1;
	unsigned int seed = 1234;
	std::cout<<"******montercarlo start "<<points<<" times ******"<<std::endl;
	gettimeofday(&t1,NULL);
	while (count_total < points) {
		#if USE_RAND
		rx = rand_r(&seed)/(RAND_MAX+1.0);
		ry = rand_r(&seed)/(RAND_MAX+1.0);
		#else
		rx = rand_array[count_total%RAND_ARRAY_SIZE];
		ry = rand_array[(count_total+1)%RAND_ARRAY_SIZE];
		#endif
		if (rx*rx + ry*ry <= 1.0) {
			count_in++;
		}
		count_total++;
    }
	gettimeofday(&t2,NULL);
    timeuse1 = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
	std::cout<<"******montercarlo loop end, time:"<<timeuse1<<" secs," <<count_in<< "in ******"<<std::endl;
	return 0;
}

int montecarlo_wrapper(unsigned long long &points,
			          unsigned long long &count_in,
					  std::mutex &mtx){
   montecarlo(points,count_in);
   mtx.unlock();
}

int montecarlo_wrapper_ch(unsigned long long &points,
			          unsigned long long &count_in,
					  co::co_chan<int> &ch){
   montecarlo(points,count_in);
   ch << 2;
}

typedef struct mon_param_t{
	unsigned long long points;
	unsigned long long count_in;
	co::co_chan<int> ch;
}mon_param;

void* montecarlo_wrapper_ch_pthread(void* param){
   mon_param *p = (mon_param *)param;
   montecarlo(p->points,p->count_in);
   (p->ch) << 2;
}

#endif 

static int dco_index =0;
std::string montecarloMap::montecarloRun(std::string a){
	std::stringstream ss(a);
    unsigned long long points;
    ss>>points;
	unsigned long long count_in = 0;
//	unsigned long long count_out = 0;
	#if SEP_THREAD
    void* proc = co_sched.GetCurrentProc();
	if(proc){
        co_sched.detachSched(proc);
	}else{
		std::cout<<"obtain proc failed"<<std::endl;
	}
	#endif //#if SEP_THREAD
    #if USE_CO
    montecarlo(points,count_in);
	#endif //#if USE_CO 
	#if USE_PROC
	pid_t pid = fork();
    if (pid < 0){
        printf("error\n");
        return std::string("");
    }
    else if (pid == 0){
        printf("fork success,this is son process\n");
		unsigned long long cnt_in;
        void* pshm;
		int shmid;
		int semid;
		int ret = getSharedMem("/dev/null", 111, 4096, &shmid, &pshm);
		if(ret<0){
			std::cout<<"shm get error!"<<std::endl;
			exit(0);
		}
		ret = getSem("/tmp",111,0,&semid);
        montecarlo(points,cnt_in);
		memcpy(pshm,&cnt_in,sizeof(cnt_in));
		postSem(semid);
		std::cout<< "child proc in-pionts cnt:"<<cnt_in<<std::endl;
		exit(0);
    }
    else{
        printf("fork success,this is father process,son process id is %d \n",pid);
		void* pshm;
		int shmid;
		int semid;
		int ret = getSharedMem("/dev/null", 111, 4096, &shmid, &pshm);
		if(ret<0){
			std::cout<<"shm get error in main proc!"<<std::endl;
		}
		ret = getSem("/tmp",111,0,&semid);
		waitSem(semid);
		unsigned long long *p = (unsigned long long *)pshm;
		std::cout<< "main proc get in-pionts cnt:"<<*p<<std::endl;
		count_in = *p; 
        rmShareMem(shmid);
		rmSem(semid);
    }
    #endif //#if USE_PROC

    #if USE_THREAD_JOIN
	std::thread tm(montecarlo, std::ref(points), std::ref(count_in));
    tm.join(); 
	#endif//  #if USE_THREAD_JION

	#if USE_THREAD_MTX
    std::mutex mtx;
	std::thread tm(montecarlo_wrapper, std::ref(points), std::ref(count_in), std::ref(mtx));
	tm.detach();
	mtx.lock();
	mtx.lock();
	#endif //#if USE_THREAD_MTX

    #if USE_THREAD_CH
    co::co_chan<int> ch;
	int tmp;
	std::thread tm(montecarlo_wrapper_ch, std::ref(points), std::ref(count_in), std::ref(ch));
	tm.detach();
    ch >> tmp;
	#endif //#if USE_THREAD_CH
	
	#if USE_PTHREAD_CH
    //co::co_chan<int> ch;
	mon_param param;
	param.points = points;
	param.count_in =0;
	int tmp;
    //void *res;
    pthread_t threads;
    //int th;
    struct sched_param sp;
    bzero((void*)&sp, sizeof(sp));
    sp.sched_priority = sched_get_priority_max(SCHED_FIFO);

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(dco_index++, &cpuset);
	if(dco_index>=16){
		dco_index=0;
	}
	pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	pthread_attr_setschedparam(&attr, &sp);
	pthread_create(&threads, &attr, montecarlo_wrapper_ch_pthread, (void*)&param);
	pthread_detach(threads);
	param.ch >> tmp;
	count_in = param.count_in;
	#endif//#if USE_PTHREAD_CH

	DdoDataId did= UhconnSimpleDB::getInstance().genDataId();
	DdoBlockData d;
	d.len = sizeof(unsigned long long)*2;
	d.data = malloc(d.len);
	unsigned long long * p =(unsigned long long *)d.data;
	*p = count_in;
	*(p+1) = points - count_in;
	UhconnSimpleDB::getInstance().storeBlock(did, d);
	free(d.data);
	std::stringstream s2;
    s2 << did;
    #if SEP_THREAD
	if(proc)
	{
        co_sched.attachSched(proc);
	}
	#endif //#if SEP_THREAD
    return s2.str();

}

montecarloMap::montecarloMap()
{
    getFunctionTable()["montecarlo"] = (PTR)&montecarloMap::montecarloRun;
	#if !USE_RAND
	if(!is_rand_array_init){ // not safe, do not care it
		gen_rand(); 
	}
	#endif
}

montecarloMap::~montecarloMap(){

}