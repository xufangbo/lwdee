#include "driver.h"

#include <vector>

#include "core/DcoFactory.h"
#include "core/Partition.h"

#include "worker/StageTask.h"

using namespace std;

int main() {

  string fileName = "/home/kevin/git/lwdee/edc/harry-potter.txt";

  vector<PartitionInput> partitionInputs;
  for (int i = 0; i < 4; i++) {
    PartitionInput partition(i,fileName);

    partitionInputs.push_back(partition);
  }
  
  for(PartitionInput &p : partitionInputs){
    StageTask().run(&p);
  }

//   UhconnDco *dco = DcoFactory::newDco("");

  cout << "driver ..." << endl;
  return 0;
}
