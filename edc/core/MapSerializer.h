#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "ByteSpan.h"

using namespace std;

typedef pair<string, int> Pair;
typedef map<string, int> Map;
typedef shared_ptr<Map> Map_ref;

class MapSerializer {
 private:
  int totalSize(map<string,int> *tuples);

 public:
  ByteSpan_ref serailize(Map *tuples);
  Map_ref deserailize(ByteSpan *bytes);
};
