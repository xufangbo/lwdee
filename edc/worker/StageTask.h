#pragma once
#include "Task.h"

class StageTask : public Task {
private:
	void runTask() override {
		cout << "run stage task" << endl;
	};
};