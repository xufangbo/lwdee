#pragma once
#include "Task.h"

class ResultTask : public Task {
private:
	void runTask(Partition *p) override {
		cout << "run result task" << endl;
	};
};