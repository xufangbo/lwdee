// edc.cpp: 定义应用程序的入口点。
//

#include "worker.h"
#include "StageTask.h"
#include "ResultTask.h"

using namespace std;

int main()
{
	StageTask().run();
	ResultTask().run();
	return 0;
}
