#include <vector>
#include "driver.h"
#include "core/Partition.h"

using namespace std;

int main()
{
	string fileName = "/usr/home/test/harrypot.txt";

	vector<PartitionInput> partitionInputs;
	for (int i = 0; i < 4; i++) {
		PartitionInput partition(i);
		{
			partition.fileName = fileName;
		}

		partitionInputs.push_back(i);
	}

	cout << "driver ..." << endl;
	return 0;
}
