#pragma once
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

class Vector {
 public:
  template <class T>
  static void foreach (vector<T> *ss, function<void(T)> fun) {
    for_each(ss->begin(), ss->end(), fun);
  }
};