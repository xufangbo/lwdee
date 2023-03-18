#include "driver.h"

#include <vector>

#include "core/Partition.h"
#include "worker/ResultTask.h"
#include "worker/StageTask.h"

using namespace std;

int main() {
  string fileName = "/home/kevin/git/lwdee/edc/harry-potter.txt";

  int splitNums = 4;

  PartitionInput partitionInputs[splitNums];
  for (int i = 0; i < splitNums; i++) {
    partitionInputs[i] = PartitionInput(i, fileName);
  }

  PartitionStage partitionStages[splitNums];
  for (int i = 0; i < splitNums; i++) {
    DDO ddo = StageTask().run(partitionInputs + i);
    partitionStages[i] = PartitionStage(i, ddo);
  }

  DDO results[splitNums];
  for (int i = 0; i < splitNums; i++) {
    DDO ddo = ResultTask().run(partitionStages + i);
    results[i] = ddo;
  }

  cout << "driver ..." << endl;
  return 0;
}
