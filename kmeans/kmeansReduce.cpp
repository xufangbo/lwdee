// Author: Felix Duvallet

#include "kmeansReduce.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iterator>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <sys/time.h>

using namespace std;

KMeansReduce::KMeansReduce(int k, int max_iterations):KMeans(k,max_iterations)
{
}


bool KMeansReduce::init_mr(int cluster_num, int dim){
    num_clusters_ = cluster_num;
    dimension = dim;
    return true;
}

int KMeansReduce::measFromStepResult(std::vector<std::vector<std::pair<Point,int>>> &results){
  int split_num = results.size();
  int cluster_num = results[0].size();
  int dim = results[0][0].first.dimensions_;
  //std::cout<<"split_num:"<<split_num<<" cluster_num:"<<cluster_num<<" dim:"<<dim<<endl;
  //std::vector<Point> newMeans; 
  Point p(dim,true);
  int tn;
  struct timeval t1,t2;
  double timeuse;
  get_means().clear();
  gettimeofday(&t1,NULL);
  for(int i=0; i<cluster_num; i++){
    get_means().push_back(p);
  }
  gettimeofday(&t2,NULL);
  timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
  cout<<"reduce create new means cost time = "<<timeuse<<"secs"<<endl;

  gettimeofday(&t1,NULL);
  for(int i=0; i<cluster_num; i++){
    tn =0;
    for(int j=0; j<split_num; j++){
      //std::cout << "add result["<<j<<"]["<<i<<"]"<<endl;
      get_means()[i].add(results[j][i].first);
      tn += results[j][i].second;
    }
    get_means()[i].div(tn);
  //  std::cout << "total_points: "<<tn<< "  cluster:"<<i<<endl;
  }
  gettimeofday(&t2,NULL);
  timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
  cout<<"cal new means cost time = "<<timeuse<<"secs"<<endl;
  return 0;
}

 bool KMeansReduce::update_means(){
    measFromStepResult(mid_val);
    mid_val.clear();
    return true;
 }

#ifdef KMEANS_PIONT_FLOAT
void KMeansReduce::resultFromBuf(void*buf, int len){
    int il = dimension*sizeof(float)+sizeof(int);
    size_t count = len/il;
    if(count != num_clusters_){
      std::cout<<"bad result buf, cluster:"<<count<<" expect:"<<num_clusters_;
    }
    std::vector<std::pair<Point,int>> res;
    for(size_t i=0;i<count;i++){
      void*p=(void*)((unsigned char*)buf+(il*i));
      int *n=(int*)((unsigned char*)buf+(il*i)+dimension*sizeof(float));
      res.push_back(std::pair<Point,int>(Point(p,il),*n));
    }
    mid_val.push_back(res);
}
#else
void KMeansReduce::resultFromBuf(void*buf, int len){
    int il = dimension*sizeof(double)+sizeof(int);
    size_t count = len/il;
    if(count != num_clusters_){
      std::cout<<"bad result buf, cluster:"<<count<<" expect:"<<num_clusters_;
    }
    std::vector<std::pair<Point,int>> res;
    for(size_t i=0;i<count;i++){
      void*p=(void*)((unsigned char*)buf+(il*i));
      int *n=(int*)((unsigned char*)buf+(il*i)+dimension*sizeof(double));
      res.push_back(std::pair<Point,int>(Point(p,il),*n));
    }
    mid_val.push_back(res);
}
#endif // KMEANS_PIONT_FLOAT

  void KMeansReduce::printResult(void){
    int len1 = mid_val.size();
    std::cout << "result(reduce):"<<endl;
    for(int i=0; i<len1; i++){
      int len2 = mid_val[i].size();
      for(int j=0;j<len2;j++){
        std::cout << mid_val[i][j].first << endl;
        std::cout <<"count:" << mid_val[i][j].second<<endl;
      }
      std::cout << "------------------------------"<<endl;
    }

  }