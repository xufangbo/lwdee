#pragma once
#include <iostream>
#include <memory>
#include <vector>

#include "ByteSpan.h"

using namespace std;

typedef tuple<string, int> Tuple;
typedef vector<Tuple> Tuples;
typedef shared_ptr<Tuples> Tuples_ref;

class TuplesSerialzer {
 private:
  int totalSize(Tuples *tuples);

 public:
  ByteSpan_ref serailize(Tuples *tuples);
  Tuples_ref deserailize(ByteSpan *bytes);
};
