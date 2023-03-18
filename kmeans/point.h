// Basic storage class for an n-dimensional point with a cluster assignment.
//
// Author: Felix Duvallet

#ifndef __KMEANS_POINT_H__
#define __KMEANS_POINT_H__

#include <vector>
#include <iostream>

#ifdef KMEANS_PIONT_FLOAT

class Point{
 public:
  Point(){}

  // Initialize the number of dimensions, optionally set all values to zero.
  Point(int num_dimensions, bool init_zeros = true);

  Point(float x, float y, float z);

  // Initialize from a vector.
  Point(const std::vector<float> &vector);
// buf MUST contains doubles
  Point(void* buf, int len);

  ~Point() { }

  // Compute distance between two points.
  static double distance(const Point &p1, const Point &p2);

  // Adds a point to the current point.
  void add(const Point &point);

  void div(const int n);

  // Update the cluster assignment. Returns true if cluster assignment
  // changed, false if it stayed the same.
  bool update(int k);

 //data len in bytes
  int dataLen(void);

  int dim(void);

//dump data to buf, return counts of double, input the buf and buf len in bytes
  int toBuffer(void* buf, int bufLen);
  // Members: the data, the number of dimensions, and the cluster ID.
  std::vector<float> data_;
  int dimensions_;
  int cluster_;

  friend std::ostream &operator<<(std::ostream &target, const Point &point);
};

#else

class Point {
 public:
  Point() { }

  // Initialize the number of dimensions, optionally set all values to zero.
  Point(int num_dimensions, bool init_zeros = true);

  Point(double x, double y, double z);

  // Initialize from a vector.
  Point(const std::vector<double> &vector);
// buf MUST contains doubles
  Point(void* buf, int len);

  ~Point() { }

  // Compute distance between two points.
  static double distance(const Point &p1, const Point &p2);

  // Adds a point to the current point.
  void add(const Point &point);

  void div(const int n);

  // Update the cluster assignment. Returns true if cluster assignment
  // changed, false if it stayed the same.
  bool update(int k);

 //data len in bytes
  int dataLen(void);

  int dim(void);

//dump data to buf, return counts of double, input the buf and buf len in bytes
  int toBuffer(void* buf, int bufLen);
  // Members: the data, the number of dimensions, and the cluster ID.
  std::vector<double> data_;
  int dimensions_;
  int cluster_;

  friend std::ostream &operator<<(std::ostream &target, const Point &point);
};
#endif //KMEANS_PIONT_FLOAT

#endif  // __KMEANS_POINT_H_
