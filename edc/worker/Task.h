#pragma once

#include <iostream>

using namespace std;

class Task {
protected:
	virtual void runTask() = 0;
public:
	void run() { this->runTask(); };
};