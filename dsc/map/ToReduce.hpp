#pragma once

#include <thread>
#include <vector>
#include "lwdee/lwdee.h"

using namespace std;

class ToReduce {
 private:
  std::vector<DCO> reduceDcos;
  std::hash<std::string> _hash;

 public:
  void create_dcos();
  void send(vector<string>& words);

 private:
  string json(vector<string>& words);

 private:
  queue<pair<DDOId, DCO>> ddoIds;
  thread releaseThread;

  void releaseDdo();
};