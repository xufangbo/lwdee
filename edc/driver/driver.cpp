#include "driver.h"

#include <algorithm>
#include <vector>

#include "core/Partition.h"
#include "worker/Step1Task.h"
#include "worker/Step2Task.h"

using namespace std;

void step1();
void step2();
void collect();

int main() {
  string fileName = "/home/kevin/git/lwdee/edc/harry-potter.txt";

  int splitNums1 = 4;
  int splitNums2 = 2;

  PartitionStep1 step1Inputs[splitNums1];
  for (int i = 0; i < splitNums1; i++) {
    step1Inputs[i] = PartitionStep1(i, fileName, splitNums2);
  }

  // step1
  PartitionStep1 step1Outputs[splitNums1];
  for (int i = 0; i < splitNums1; i++) {
    PartitionStep1 step1Output = Step1Task().run(step1Inputs + i);
    step1Outputs[i] = step1Output;
  }

  // step2
  PartitionStep2 step2Inputs[splitNums2];
  for (int i = 0; i < splitNums2; i++) {
    step2Inputs[i] = PartitionStep2(i);

    for (int j = 0; j < splitNums1; j++) {
      step2Inputs[i].ddos.push_back(step1Inputs[j].ddos[i]);
    }
  }

  DDO step2Outpus[splitNums2];
  for (int i = 0; i < splitNums2; i++) {
    DDO ddo = Step2Task().run(step2Inputs + i);
    step2Outpus[i] = ddo;
  }

  // collect后合并结果
  Tuples list;
  TuplesSerialzer tuplesSerializer;
  for (int i = 0; i < splitNums2; i++) {
    DDO ddo = step2Outpus[i];
    ByteSpan_ref bytes = ddo.read();
    ddo.release();
    Tuples_ref rc = tuplesSerializer.deserailize(bytes.get());

    for_each(rc->begin(), rc->end(), [&list](Tuple &t) { list.push_back(t); });
  }

  for (Tuple &i : list) {
    cout << get<0>(i) << " " << get<1>(i) << endl;
  }

  return 0;
}
