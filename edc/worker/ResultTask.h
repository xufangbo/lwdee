#pragma once
#include "Task.h"

class ResultTask : public Task {
private:
	void runTask() override {
		cout << "run result task" << endl;
	};
};