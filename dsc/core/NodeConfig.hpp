#pragma once

#include <iostream>
#include <vector>

class TNode {
 public:
  std::string name;
  int nodeId;
  std::string ip;
  int port;
  int dport;
  int mport;
};

class NodeConfig {
  public:
  static std::vector<TNode> nodes;
  static void readConfig();
  static int nodeCount() { return nodes.size(); }
};