#pragma once
#include<iostream>

using namespace std;

class Partition {
public:
	Partition(int index) :index(index) {}
	int index;
};

class PartitionInput : public Partition {
public:
	PartitionInput(int index):Partition(index) {}
	string fileName;
};

// class PartitionStage : public Partition {
// public:
// 	PartitionStage(int index) :Partition(index) {}
// 	DDO ddo;
// };

// class PartitionResult : public Partition {
// public:
// 	PartitionResult(int index) :Partition(index) {}
// 	DDO ddo;
// };