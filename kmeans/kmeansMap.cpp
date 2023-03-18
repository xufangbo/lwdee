// Author: Felix Duvallet

#include "kmeansMap.h"

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

KMeansMap::KMeansMap(int k, int max_iterations, int dim):KMeans(k,max_iterations)
{
    this->dim = dim;
}

bool KMeansMap::init_mr(const std::vector<Point> &points,
                     const std::vector<Point> &means) {
  for (const auto &p : points) {
    points_.push_back(p);
  }

  means_= means;
  return true;
}


bool KMeansMap::runOneStep() {
    struct timeval t1,t2;
    double timeuse;
    #if CAL_SUM_DIRECT
    init_result();
    #endif
    gettimeofday(&t1,NULL);
    bool changed = assign();
    gettimeofday(&t2,NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    cout<<"assign() cost time = "<<timeuse<<"secs"<<endl;
    #if !CAL_SUM_DIRECT
    gettimeofday(&t1,NULL);
    update_sums();
    gettimeofday(&t2,NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    cout<<"update_sums() cost time = "<<timeuse<<"secs"<<endl;
    #endif
    if (!changed) {
      // cout << "KMeans has converged" << endl;
      return true;
    }
    return false;
}
#if CAL_SUM_DIRECT
bool KMeansMap::init_result(){
    Point p(points_[0].dimensions_,true);
    result.clear();
    for (size_t cluster_idx = 0; cluster_idx < num_clusters_; ++cluster_idx) {
      result.push_back(std::pair<Point,int>(p,0));
    }
    return true;
}
#endif 

bool KMeansMap::update_sums() {
  // Compute the mean of the points in that cluster.

  // First, compute a map of the cluster assignments. This prevents us from
  // iterating over the data k times.
  std::multimap<int, const Point *> point_cluster_map;
  for (const auto &point : points_) {
    // Map is cluster_index -> Point*
    auto pair = std::pair<int, const Point *>(point.cluster_, &point);
    point_cluster_map.insert(pair);
  }
#if !CAL_SUM_DIRECT
 // init result
  std::cout << "KMeansMap::update_sums points_.size="<<points_.size()<<std::endl;
  Point p(points_[0].dimensions_,true);
  result.clear();
  for (size_t cluster_idx = 0; cluster_idx < num_clusters_; ++cluster_idx) {
    result.push_back(std::pair<Point,int>(p,0));
  }
 #endif

  // Iterate over each cluster, computing the mean.
  for (size_t cluster_idx = 0; cluster_idx < num_clusters_; ++cluster_idx) {
    computeClusterSum(point_cluster_map, cluster_idx,
                      &(result[cluster_idx].first),
                      &(result[cluster_idx].second)
                      );
    // std::cout << "cluster:"<<cluster_idx<< " count:"<<result[cluster_idx].second <<std::endl;
  }
  return true;
}



void KMeansMap::computeClusterSum(
  const std::multimap<int, const Point *> &multimap,
  int cluster,
  Point *sum,
  int *points_num) {
  // Zero-out the mean.
  for (int dim = 0; dim < sum->dimensions_; ++dim)
    sum->data_[dim] = 0.0;

  *points_num =0;
  // Find all the points in the given cluster, this returns an iterator pair
  // (begin and end).
  auto in_cluster = multimap.equal_range(cluster);
  int num_points = 0;

  // Compute the mean: sum over all points, then divide by the number.
  for (auto itr = in_cluster.first; itr != in_cluster.second; ++itr) {
    sum->add(*(itr->second));
    num_points += 1;
  }
  *points_num = num_points;

}

  int KMeansMap::resultLen(void){
    int il = result[0].first.dataLen()+sizeof(int);
    return il*result.size();
  }

  void KMeansMap::resultToBuf(void*buf, int bufLen){
    int il = result[0].first.dataLen()+sizeof(int);
    for(size_t i=0;i<result.size();i++){
      if( (i+1)*il > static_cast<size_t>(bufLen) ){ break;}
      result[i].first.toBuffer((unsigned char*)buf+il*i,il);
      *(int*)((unsigned char*)buf+il*i+result[i].first.dataLen())= result[i].second;
    } 
  }

  void KMeansMap::printResult(void){
    int len = result.size();
    std::cout << "result(map):"<<endl;
    for(int i=0;i<len;i++){
      std::cout << result[i].first << endl;
      std::cout <<"count:" << result[i].second<<endl;
    }
  }

