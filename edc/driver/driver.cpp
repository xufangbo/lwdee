#include "driver.h"

#include <vector>
#include <algorithm>

#include "core/Partition.h"
#include "worker/ResultTask.h"
#include "worker/StageTask.h"

using namespace std;

int main() {
  string fileName = "/home/kevin/git/lwdee/edc/harry-potter.txt";

  int splitNums1 = 4;
  int splitNums2 = 2;

  PartitionInput partitionInputs[splitNums1];
  for (int i = 0; i < splitNums1; i++) {
    partitionInputs[i] = PartitionInput(i, fileName);
  }

  // step1
  PartitionStage partitionStages[splitNums1];
  for (int i = 0; i < splitNums1; i++) {
    DDO ddo = StageTask().run(partitionInputs + i);
    partitionStages[i] = PartitionStage(i, ddo);
  }

  // step2
  DDO results[splitNums1];
  for (int i = 0; i < splitNums1; i++) {
    DDO ddo = ResultTask().run(partitionStages + i);
    results[i] = ddo;
  }

  //
  Tuples list;
  TuplesSerialzer tuplesSerializer;
  for (int i = 0; i < splitNums1; i++) {
    DDO ddo = results[i];
    ByteSpan_ref bytes = ddo.read();
    ddo.release();
    Tuples_ref rc = tuplesSerializer.deserailize(bytes.get());

    for_each(rc->begin(), rc->end(), [&list](Tuple &t) { list.push_back(t); }) ;
  }

  for(Tuple &i : list){
    cout << get<0>(i) << " " << get<1>(i) << endl;
  }
  
  return 0;
}
