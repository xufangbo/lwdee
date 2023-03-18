// Author: Felix Duvallet

#include "kmeans.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iterator>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;
#define KMEANS_DEBUG 0
KMeans::KMeans(int k, int max_iterations)
  : num_clusters_(k),
    max_iterations_(max_iterations) {
}

bool KMeans::init(const std::vector<Point> &points) {
  // Store all points and create a vector that looks like [0, 1, 2, ... N]
  std::vector<int> points_indices;
  int point_num = 0;
  points_.clear();

  for (const auto &p : points) {
    points_.push_back(p);
    points_indices.push_back(point_num);
    point_num += 1;
  }

  // Initialize the means randomly: shuffle the array of unique index
  // integers. This prevents assigning the mean to the same point twice.
  std::random_device rd;
  std::mt19937 rng(rd());
  std::shuffle(points_indices.begin(), points_indices.end(), rng);

  // Sanity check
  assert(points.size() >= num_clusters_);

  for (size_t idx = 0; idx < num_clusters_; ++idx) {
    Point mean(points[points_indices[idx]]);
    means_.push_back(mean);
  }

  return true;
}


bool KMeans::run() {
  for (size_t iteration = 1; iteration <= max_iterations_; ++iteration) {
    cout << "== KMeans iteration " << iteration << " == " << endl;
    bool changed = assign();
    update_means();

    if (!changed) {
      cout << "KMeans has converged after " << iteration <<  " iterations."
           << endl;
      return true;
    }
  }
  return false;
}


int KMeans::findNearestCluster(const Point &point) {
  double min_dist = 1e12;
  int min_cluster = -1;

  for (size_t idx = 0; idx < num_clusters_; ++idx) {
    const double dist = Point::distance(point, means_[idx]);
    if (dist < min_dist) {
      min_dist = dist;
      min_cluster = idx;
    }
  }

  return min_cluster;
}

bool KMeans::assign() {
  bool changed = false;
  // Assign each point to the nearest cluster: iterate over all points, find
  // the nearest cluster, and assign.
  std::cout<<"KMeans::assign points_.size()= "<<points_.size()<<std::endl;
  for (auto &point : points_) {
    const int new_cluster = findNearestCluster(point);

    // set changed to true if the cluster was updated. Note that we cannot
    // inline (changed = changed || update) since the compiler will
    // 'optimize out' the update step.
    bool ret = point.update(new_cluster);
    changed = changed || ret;
    //update sum here
    result[new_cluster].first.add(point);
    result[new_cluster].second++;
#if KMEANS_DEBUG
    cout << "Assigned point " << point << " to cluster: "
    << new_cluster << endl;
#endif
  }
  return changed;
}


bool KMeans::update_means() {
  // Compute each mean as the mean of the points in that cluster.

  // First, compute a map of the cluster assignments. This prevents us from
  // iterating over the data k times.
  std::multimap<int, const Point *> point_cluster_map;
  for (const auto &point : points_) {
    // Map is cluster_index -> Point*
    auto pair = std::pair<int, const Point *>(point.cluster_, &point);
    point_cluster_map.insert(pair);
  }

  // Iterate over each cluster, computing the mean.
  for (size_t cluster_idx = 0; cluster_idx < num_clusters_; ++cluster_idx) {
    computeClusterMean(point_cluster_map, cluster_idx,
                       &means_[cluster_idx]);
  }
  return true;
}


void KMeans::computeClusterMean(
  const std::multimap<int, const Point *> &multimap,
  int cluster,
  Point *mean) {
  // Zero-out the mean.
  for (int dim = 0; dim < mean->dimensions_; ++dim)
    mean->data_[dim] = 0.0;

  // Find all the points in the given cluster, this returns an iterator pair
  // (begin and end).
  auto in_cluster = multimap.equal_range(cluster);
  int num_points = 0;

  // Compute the mean: sum over all points, then divide by the number.
  for (auto itr = in_cluster.first; itr != in_cluster.second; ++itr) {
    mean->add(*(itr->second));

    num_points += 1;
  }

  for (int idx = 0; idx < mean->dimensions_; ++idx) {
    mean->data_[idx] /= float(num_points);
  }
}

void KMeans::printMeans() {
  for (const auto &mean : means_) {
    cout << "Mean: " << mean << endl;
  }
}

// static
#ifdef KMEANS_PIONT_FLOAT
bool KMeans::loadPoints(const string &filepath, vector<Point> *points) {
  std::ifstream file_stream(filepath, std::ios_base::in);
  if (!file_stream) {
    cout << "Could not open file " << filepath << endl;
    return false;
  }else{
    cout <<"load point from file "<<filepath<<".."<<endl;
  }

  std::string line;
  // Split up each line of the file.
  while (getline(file_stream, line, '\n')) {
    std::stringstream line_stream(line);

    // Get a vector of numbers directly from a stream iterator.
    std::istream_iterator<float> start(line_stream), end;
    std::vector<float> numbers(start, end);

    Point p(numbers);
    points->push_back(p);
  }

  return true;
}

#else
bool KMeans::loadPoints(const string &filepath, vector<Point> *points) {
  std::ifstream file_stream(filepath, std::ios_base::in);
  if (!file_stream) {
    cout << "Could not open file " << filepath << endl;
    return false;
  }else{
    cout <<"load point from file "<<filepath<<".."<<endl;
  }

  std::string line;
  // Split up each line of the file.
  while (getline(file_stream, line, '\n')) {
    std::stringstream line_stream(line);

    // Get a vector of numbers directly from a stream iterator.
    std::istream_iterator<double> start(line_stream), end;
    std::vector<double> numbers(start, end);

    Point p(numbers);
    points->push_back(p);
  }

  return true;
}
#endif //#ifdef KMEANS_PIONT_FLOAT

#define PATH_DATA "/mnt/data/x"

bool KMeans::loadPoints(const int file_number_start, int file_count, std::vector<Point> *points) {
  // const char* file_path_base = PATH_DATA;
  int loop = file_number_start;
  std::string ss = PATH_DATA;
  std::cout<<"KMeans::loadPoints index from "<<file_number_start <<" count:"<<file_count<<std::endl;
  while(loop < file_number_start + file_count) {
    std::stringstream fp;
    if( loop != 0 ) {
      fp << loop;
    }
    ++loop;
    loadPoints(ss + fp.str(), points);
    fp.clear();
  }
  return true;

}

#ifdef KMEANS_PIONT_FLOAT
void KMeans::writeMeans(const std::string &filepath) {
  std::ofstream file_stream(filepath, std::ios_base::out);
  if (!file_stream) {
    cout << "Could not open file " << filepath << endl;
    return;
  }

  // Copy all data to file_stream, then append a newline.
  for (auto &mean : means_) {
    std::ostream_iterator<float> itr(file_stream, " ");
    std::copy(mean.data_.begin(), mean.data_.end(), itr);
    file_stream << endl;
  }
  return;

}
#else
void KMeans::writeMeans(const std::string &filepath) {
  std::ofstream file_stream(filepath, std::ios_base::out);
  if (!file_stream) {
    cout << "Could not open file " << filepath << endl;
    return;
  }

  // Copy all data to file_stream, then append a newline.
  for (auto &mean : means_) {
    std::ostream_iterator<double> itr(file_stream, " ");
    std::copy(mean.data_.begin(), mean.data_.end(), itr);
    file_stream << endl;
  }
  return;

}
#endif //#ifdef KMEANS_PIONT_FLOAT

 int KMeans::meansLen(void){
  return means_.size()*means_[0].dataLen();
 }

 // return len in bytes
 int KMeans::pointsLen(void){
  return points_.size()*points_[0].dataLen();
 }

int KMeans::size(std::vector<Point> &vp){
  return vp.size()*vp[0].dataLen();
}

 int KMeans::toBuf(std::vector<Point> &vp,void* buf, int bufLen){
  int pl = vp[0].dataLen();
  for(size_t i=0;i<vp.size();i++){
    if((i+1)*pl>static_cast<size_t>(bufLen)){
      break;
    }
    vp[i].toBuffer((unsigned char*)buf+i*pl, pl);
  }
  return 0;
 }

void KMeans::pointsToBuf(void* buf, int bufLen){
  toBuf(points_,buf,bufLen);
 }

 //  input buf and len in bytes
void KMeans::meansToBuf(void* buf, int bufLen){
  toBuf(means_,buf,bufLen);
 }

#ifdef KMEANS_PIONT_FLOAT
 void KMeans::fromBuf(std::vector<Point> &vp, void*buf, int len, int dim){
  vp.clear();
  int pl = sizeof(float)*dim;
  for(int i=0;(i+1)*pl<=len;i++){
    vp.push_back(Point((unsigned char*)buf+pl*i, pl));
  }
 }
#else
 void KMeans::fromBuf(std::vector<Point> &vp, void*buf, int len, int dim){
  vp.clear();
  int pl = sizeof(double)*dim;
  for(int i=0;(i+1)*pl<=len;i++){
    vp.push_back(Point((unsigned char*)buf+pl*i, pl));
  }
 }
#endif////#ifdef KMEANS_PIONT_FLOAT
 void KMeans::meansFromBuf(void*buf, int len, int dim){
  fromBuf(means_, buf, len, dim);
 }

 void KMeans::pointsFromBuf(void*buf, int len, int dim){
  fromBuf(points_, buf, len, dim);
 }


