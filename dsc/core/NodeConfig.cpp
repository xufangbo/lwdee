#include "NodeConfig.hpp"

#include <sstream>

#include "core/Exception.hpp"
#include "core/cjson.hpp"
#include "core/log.hpp"
#include <string.h>

std::vector<TNode> NodeConfig::nodes;
int NodeConfig::index = 0;

void NodeConfig::readConfig() {
  if (nodes.size() > 0) {
    return;
  }
  std::string configFile = "./conf.json";
  FILE *fp = fopen(configFile.c_str(), "r");
  if (fp != NULL) {
    fclose(fp);
  } else {
    configFile = "./config/conf.json";
    fp = fopen(configFile.c_str(), "r");
    if (fp != NULL) {
      fclose(fp);
    } else {
      configFile = "/home/kevin/git/lwdee/config/conf.json";
    }
  }

  std::string json;

  fp = fopen(configFile.c_str(), "r");
  if (fp == NULL) {
    logger_error("can't open dsc.json, %s", configFile.c_str());
  }
  char c = 0;
  while ((c = fgetc(fp)) != EOF) {
    json += c;
  }

  fclose(fp);

  cJSON *root = cJSON_Parse(json.c_str());

  cJSON *node = root->child;
  while (node != NULL) {
    if (node->type != cJSON_Object) {
      continue;
    }
    TNode tnode;

    tnode.name = node->string;
    tnode.ip = cJSON_GetObjectItem(node, "ip")->valuestring;
    tnode.nodeId = cJSON_GetObjectItem(node, "nodeId")->valueint;
    tnode.dport = cJSON_GetObjectItem(node, "dataPort")->valueint;
    tnode.mport = cJSON_GetObjectItem(node, "msgPort")->valueint;

    tnode.port = tnode.mport;
    nodes.push_back(tnode);

    node = node->next;
  }

  logger_debug("%d config nodes", nodes.size());
}

TNode *NodeConfig::nextNode() {
  index++;
  if (index >= nodes.size()) {
    index = 1;
  }

  return nodes.data() + index;
}

std::string NodeConfig::voxorId(TNode *node, int index) {
  return std::to_string(node->nodeId) + " " + std::to_string(index);
}

VoxorId NodeConfig::voxorId(std::string input) {
  VoxorId id;
  std::stringstream out(input);
  out >> id.nodeId;
  out >> id.voxorKey;

  return id;
}

TNode *NodeConfig::byNodeId(int id) {
  for (auto &node : nodes) {
    if (node.nodeId == id) {
      return &node;
    }
  }

  throw Exception("can't find node by id " + std::to_string(id), ZONE);
}

TNode *NodeConfig::byName(std::string name){
  for (auto &node : nodes) {
      if (strcmp(node.name.c_str(),name.c_str())==0) {
        return &node;
      }
    }

  throw Exception("can't find node by name " + name, ZONE);
}