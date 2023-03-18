// C++ implementation of K-Means clustering for n-dimensional data.
//
// This class follows the standard Expectation-Maximization iterative algorithm,
// also known by Lloyd's algorithm:
//   Repeat until convergence:
//      - Assign all points to the nearest cluster.
//      - Compute the cluster mean based on its assigned points.
//   Convergence is when the cluster assignment doesn't change.
//   The initialization of the means uses k random points from the data.
//
// Author: Felix Duvallet

#ifndef __KMEANS_KMEANSREDUCE_H__
#define __KMEANS_KMEANSREDUCE_H__

#include <map>
#include <string>
#include <vector>

#include "point.h"
#include "kmeans.h"

class KMeansReduce: public KMeans {
 public:
  // K is the number of clusters we want. Max iterations is just to prevent
  // running forever.
  KMeansReduce(int k = 2, int max_iterations = 100);

  bool init_mr(int cluster_num, int dim);
  // Compute the means to be the average of all the points in each cluster.
  bool update_means();

  void resultFromBuf(void*buf, int len);

  void printResult(void);

  int measFromStepResult(std::vector<std::vector<std::pair<Point,int>>> &results);
  std::vector<std::vector<std::pair<Point,int>>> mid_val;
  int dimension;
};

#endif  // __KMEANS_KMEANSREDUCE_H__
