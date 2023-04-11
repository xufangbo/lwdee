#include "Partition.h"
#include "core/cjson.hpp"

std::string PartitionKafka::toJson() {
  cJSON* root = cJSON_CreateObject();

  cJSON_AddNumberToObject(root, "index", index);
  cJSON_AddStringToObject(root, "group", group.c_str());
  cJSON_AddStringToObject(root, "topic", topic.c_str());
  cJSON_AddNumberToObject(root, "window", window);

  cJSON* voxorNodes = cJSON_CreateArray();
  cJSON_AddItemToObject(root, "mapVoxors", voxorNodes);
  for (auto& i : this->mapVoxors) {
    auto item = cJSON_CreateString(i.c_str());
    cJSON_AddItemToArray(voxorNodes, item);
  }

  char* jsonText = cJSON_Print(root);

  return jsonText;
}

void PartitionKafka::fromJson(std::string* json) {
  cJSON* node = cJSON_Parse(json->c_str());
  index = cJSON_GetObjectItem(node, "index")->valueint;
  group = cJSON_GetObjectItem(node, "group")->valuestring;
  topic = cJSON_GetObjectItem(node, "topic")->valuestring;
  window = cJSON_GetObjectItem(node, "window")->valueint;

  cJSON* voxorNodes = cJSON_GetObjectItem(node, "mapVoxors");
  int size = cJSON_GetArraySize(voxorNodes);
  for (int i = 0; i < size; i++) {
    auto voxorId = cJSON_GetArrayItem(voxorNodes, i)->valuestring;
    this->mapVoxors.push_back(voxorId);
  }
}

std::string PartitionMap::toJson() {
  cJSON* root = cJSON_CreateObject();

  cJSON_AddNumberToObject(root, "index", index);

  cJSON* voxorNodes = cJSON_CreateArray();
  cJSON_AddItemToObject(root, "reduceVoxors", voxorNodes);
  for (auto& i : this->reduceVoxors) {
    auto item = cJSON_CreateString(i.c_str());
    cJSON_AddItemToArray(voxorNodes, item);
  }

  char* jsonText = cJSON_Print(root);

  return jsonText;
}

void PartitionMap::fromJson(std::string* json) {
  cJSON* node = cJSON_Parse(json->c_str());
  index = cJSON_GetObjectItem(node, "index")->valueint;

  cJSON* voxorNodes = cJSON_GetObjectItem(node, "reduceVoxors");
  int size = cJSON_GetArraySize(voxorNodes);
  for (int i = 0; i < size; i++) {
    auto voxorId = cJSON_GetArrayItem(voxorNodes, i)->valuestring;
    this->reduceVoxors.push_back(voxorId);
  }
}

std::string PartitionReduce::toJson() {
  cJSON* root = cJSON_CreateObject();

  cJSON_AddNumberToObject(root, "index", index);

  char* jsonText = cJSON_Print(root);

  return jsonText;
}

void PartitionReduce::fromJson(std::string* json) {
  cJSON* node = cJSON_Parse(json->c_str());
  index = cJSON_GetObjectItem(node, "index")->valueint;
}

std::string StringsSerializer::toJson(int index, vector<string>& items) {
  cJSON* root = cJSON_CreateObject();
  cJSON_AddNumberToObject(root, "index", index);

  cJSON* nodes = cJSON_CreateArray();
  cJSON_AddItemToObject(root, "items", nodes);
  for (string& i : items) {
    auto item = cJSON_CreateString(i.c_str());
    cJSON_AddItemToArray(nodes, item);
  }
  string jsonText = cJSON_Print(root);
  return jsonText;
}

int StringsSerializer::fromJson(std::string& json, vector<string>& items) {
  cJSON* root = cJSON_Parse(json.c_str());
  int index = cJSON_GetObjectItem(root, "index")->valueint;

  cJSON* nodes = cJSON_GetObjectItem(root, "items");
  int size = cJSON_GetArraySize(nodes);
  for (int i = 0; i < size; i++) {
    string line = cJSON_GetArrayItem(nodes, i)->valuestring;
    items.push_back(line);
  }

  return index;
}