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

class VoxorId {
 public:
  int nodeId;
  int voxorKey;
};

class NodeConfig {
 private:
  static int index;

 public:
  static std::vector<TNode> nodes;
  static void readConfig();
  static int nodeCount() { return nodes.size(); }
  static TNode* nextNode();
  static TNode* byNodeId(int id);
  static TNode* byName(std::string name);
  static std::string voxorId(TNode* node, int index);
  static VoxorId voxorId(std::string input);
};