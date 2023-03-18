// Author: Felix Duvallet

#include <cassert>
#include <cmath>
#include "point.h"

#ifdef KMEANS_PIONT_FLOAT

Point::Point(int num_dimensions, bool init_zeros)
  : dimensions_(num_dimensions), cluster_(-1) {
  if (init_zeros) {  // default is true.
    for (int idx = 0; idx < dimensions_; ++idx) {
      data_.push_back(0);
    }
  }
}

Point::Point(float x, float y, float z)
  : Point(3, false) {
  data_.clear();
  data_.push_back(x);
  data_.push_back(y);
  data_.push_back(z);
}

Point::Point(const std::vector<float> &vector)
  : cluster_(-1) {
  dimensions_ = (int) vector.size();
  data_.clear();
  data_.insert(data_.begin(), vector.begin(), vector.end());
}

Point::Point(void* buf, int len){
  int dim = len/sizeof(float);
  float*p = (float*)buf;
  dimensions_ = dim;
  data_.clear();
  for(int i=0;i<dim;i++){
    data_.push_back(p[i]);
  }
}

bool Point::update(int k) {
  const bool ret = cluster_ != k;
  cluster_ = k;
  return ret;
}

// static
double Point::distance(const Point &p1, const Point &p2) {
  // if(p1.dimensions_ != p2.dimensions_){
  //    std::cout << "dimension1:2("<<p1.dimensions_<<":"<<p2.dimensions_<<")"<<std::endl;
  // }
  //assert(p1.dimensions_ == p2.dimensions_);
  double dist = 0.0;

  for (int idx = 0; idx < p1.dimensions_; ++idx) {
    const float tmp = p1.data_[idx] - p2.data_[idx];
    dist += tmp * tmp;
  }
 // return sqrt(dist);
  return dist;
}

void Point::add(const Point &point) {
  assert(dimensions_ == point.dimensions_);
  for (int idx = 0; idx < dimensions_; ++idx) {
    data_[idx] += point.data_[idx];
  }
}

void Point::div(const int n) {
  for (int idx = 0; idx < dimensions_; ++idx) {
    data_[idx] /= (float)n;
  }
}

int Point::dataLen(void){
  return data_.size()*sizeof(float);
 }

int Point::dim(void){
  return dimensions_;
}

int Point::toBuffer(void *buf, int bufLen) {
  float *p = (float *)buf;
  int cnt;
  for (cnt = 0; (cnt + 1) * sizeof(float) <= bufLen; cnt++) {
    p[cnt] = data_[cnt];
  }
  return cnt;
}

std::ostream &operator<<(std::ostream &target, const Point &point) {
  target << "[";
  for (const int &d : point.data_) {
    target << d << ", ";
  }
  target << "]";
  return target;
}

#else
Point::Point(int num_dimensions, bool init_zeros)
  : dimensions_(num_dimensions), cluster_(-1) {
  if (init_zeros) {  // default is true.
    for (int idx = 0; idx < dimensions_; ++idx) {
      data_.push_back(0.0);
    }
  }
}

Point::Point(double x, double y, double z)
  : Point(3, false) {
  data_.clear();
  data_.push_back(x);
  data_.push_back(y);
  data_.push_back(z);
}

Point::Point(const std::vector<double> &vector)
  : cluster_(-1) {
  dimensions_ = (int) vector.size();
  data_.clear();
  data_.insert(data_.begin(), vector.begin(), vector.end());
}

Point::Point(void* buf, int len){
  int dim = len/sizeof(double);
  double*p = (double*)buf;
  dimensions_ = dim;
  data_.clear();
  for(int i=0;i<dim;i++){
    data_.push_back(p[i]);
  }
}

bool Point::update(int k) {
  const bool ret = cluster_ != k;
  cluster_ = k;
  return ret;
}

// static
double Point::distance(const Point &p1, const Point &p2) {
  if(p1.dimensions_ != p2.dimensions_){
     std::cout << "dimension1:2("<<p1.dimensions_<<":"<<p2.dimensions_<<")"<<std::endl;
  }
 
  assert(p1.dimensions_ == p2.dimensions_);
  double dist = 0.0;

  for (int idx = 0; idx < p1.dimensions_; ++idx) {
    const double tmp = p1.data_[idx] - p2.data_[idx];
    dist += tmp * tmp;
  }
 // return sqrt(dist);
  return dist;
}

void Point::add(const Point &point) {
  assert(dimensions_ == point.dimensions_);
  for (int idx = 0; idx < dimensions_; ++idx) {
    data_[idx] += point.data_[idx];
  }
}

void Point::div(const int n) {
  for (int idx = 0; idx < dimensions_; ++idx) {
    data_[idx] /= float(n);
  }
}

int Point::dataLen(void){
  return data_.size()*sizeof(double);
 }

int Point::dim(void){
  return dimensions_;
}

int Point::toBuffer(void *buf, int bufLen) {
  double *p = (double *)buf;
  int cnt;
  for (cnt = 0; (cnt + 1) * sizeof(double) <= static_cast<unsigned long>(bufLen); cnt++) {
    p[cnt] = data_[cnt];
  }
  return cnt;
}

std::ostream &operator<<(std::ostream &target, const Point &point) {
  target << "[";
  for (const double &d : point.data_) {
    target << d << ", ";
  }
  target << "]";
  return target;
}

#endif //#ifdef KMEANS_PIONT_FLOAT