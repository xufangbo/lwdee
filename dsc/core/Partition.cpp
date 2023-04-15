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

  char* jsonText = cJSON_PrintUnformatted(root);
  std::string rc = jsonText;

  cJSON_Delete(root);
  free(jsonText);

  return rc;
}

void PartitionKafka::fromJson(std::string* json) {
  cJSON* root = cJSON_Parse(json->c_str());
  index = cJSON_GetObjectItem(root, "index")->valueint;
  group = cJSON_GetObjectItem(root, "group")->valuestring;
  topic = cJSON_GetObjectItem(root, "topic")->valuestring;
  window = cJSON_GetObjectItem(root, "window")->valueint;

  cJSON* voxorNodes = cJSON_GetObjectItem(root, "mapVoxors");
  int size = cJSON_GetArraySize(voxorNodes);
  for (int i = 0; i < size; i++) {
    auto voxorId = cJSON_GetArrayItem(voxorNodes, i)->valuestring;
    this->mapVoxors.push_back(voxorId);
  }

  cJSON_Delete(root);
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

  char* jsonText = cJSON_PrintUnformatted(root);
  std::string rc = jsonText;

  cJSON_Delete(root);
  free(jsonText);

  return rc;
}

void PartitionMap::fromJson(std::string* json) {
  cJSON* root = cJSON_Parse(json->c_str());
  index = cJSON_GetObjectItem(root, "index")->valueint;

  cJSON* voxorNodes = cJSON_GetObjectItem(root, "reduceVoxors");
  int size = cJSON_GetArraySize(voxorNodes);
  for (int i = 0; i < size; i++) {
    auto voxorId = cJSON_GetArrayItem(voxorNodes, i)->valuestring;
    this->reduceVoxors.push_back(voxorId);
  }

  cJSON_Delete(root);
}

std::string PartitionReduce::toJson() {
  cJSON* root = cJSON_CreateObject();

  cJSON_AddNumberToObject(root, "index", index);

  char* jsonText = cJSON_PrintUnformatted(root);
  std::string rc = jsonText;

  cJSON_Delete(root);
  free(jsonText);

  return rc;
}

void PartitionReduce::fromJson(std::string* json) {
  cJSON* root = cJSON_Parse(json->c_str());
  index = cJSON_GetObjectItem(root, "index")->valueint;

  cJSON_Delete(root);
}

std::string MapInvokeData::toJson() {
  cJSON* root = cJSON_CreateObject();

  cJSON_AddNumberToObject(root, "kafkaIndex", kafkaIndex);
  cJSON_AddNumberToObject(root, "mapIndex", mapIndex);

  cJSON* itemsNode = cJSON_CreateArray();
  cJSON_AddItemToObject(root, "items", itemsNode);
  for (auto& i : *this->items) {
    cJSON* itemNode = cJSON_CreateObject();
    cJSON_AddStringToObject(itemNode, "line", i.line.c_str());
    cJSON_AddNumberToObject(itemNode, "ts", i.ts);

    cJSON_AddItemToArray(itemsNode, itemNode);
  }

  char* jsonText = cJSON_PrintUnformatted(root);
  std::string rc = jsonText;

  cJSON_Delete(root);
  free(jsonText);

  return rc;
}

void MapInvokeData::fromJson(std::string* json) {
  cJSON* root = cJSON_Parse(json->c_str());
  kafkaIndex = cJSON_GetObjectItem(root, "kafkaIndex")->valueint;
  mapIndex = cJSON_GetObjectItem(root, "mapIndex")->valueint;

  cJSON* itemsNode = cJSON_GetObjectItem(root, "items");
  int size = cJSON_GetArraySize(itemsNode);

  this->items->resize(size);
  for (int i = 0; i < size; i++) {
    auto itemNode = cJSON_GetArrayItem(itemsNode, i);
    MapRecord& record = this->items->at(i);

    record.line = cJSON_GetObjectItem(itemNode, "line")->valuestring;
    record.ts = cJSON_GetObjectItem(itemNode, "ts")->valuedouble;
  }

  cJSON_Delete(root);
}

std::string ReduceInvokeData::toJson() {
  cJSON* root = cJSON_CreateObject();

  cJSON_AddNumberToObject(root, "mapIndex", mapIndex);
  cJSON_AddNumberToObject(root, "reduceIndex", reduceIndex);

  cJSON* itemsNode = cJSON_CreateArray();
  cJSON_AddItemToObject(root, "items", itemsNode);
  for (auto& i : *this->items) {
    cJSON* itemNode = cJSON_CreateObject();
    cJSON_AddStringToObject(itemNode, "did", i.did.c_str());
    cJSON_AddNumberToObject(itemNode, "ts", i.ts);

    cJSON_AddItemToArray(itemsNode, itemNode);
  }

  char* jsonText = cJSON_PrintUnformatted(root);
  std::string rc = jsonText;

  cJSON_Delete(root);
  free(jsonText);

  return rc;
}

void ReduceInvokeData::fromJson(std::string* json) {
  cJSON* root = cJSON_Parse(json->c_str());
  mapIndex = cJSON_GetObjectItem(root, "mapIndex")->valueint;
  reduceIndex = cJSON_GetObjectItem(root, "reduceIndex")->valueint;

  cJSON* itemsNode = cJSON_GetObjectItem(root, "items");
  int size = cJSON_GetArraySize(itemsNode);

  this->items->resize(size);
  for (int i = 0; i < size; i++) {
    auto itemNode = cJSON_GetArrayItem(itemsNode, i);
    ReduceRecord& record = this->items->at(i);

    record.did = cJSON_GetObjectItem(itemNode, "did")->valuestring;
    record.ts = cJSON_GetObjectItem(itemNode, "ts")->valuedouble;
  }

  cJSON_Delete(root);
}

std::string StringsSerializer::toJson(int index, vector<string>* items) {
  cJSON* root = cJSON_CreateObject();
  cJSON_AddNumberToObject(root, "index", index);

  cJSON* nodes = cJSON_CreateArray();
  cJSON_AddItemToObject(root, "items", nodes);
  for (int i = 0; i < items->size(); i++) {
    string* str = items->data() + i;
    auto item = cJSON_CreateString(str->c_str());
    cJSON_AddItemToArray(nodes, item);
  }

  char* jsonText = cJSON_PrintUnformatted(root);
  std::string rc = jsonText;

  cJSON_Delete(root);
  free(jsonText);

  return rc;
}

int StringsSerializer::fromJson(std::string& json, vector<string>* items) {
  cJSON* root = cJSON_Parse(json.c_str());
  int index = cJSON_GetObjectItem(root, "index")->valueint;

  cJSON* nodes = cJSON_GetObjectItem(root, "items");
  int size = cJSON_GetArraySize(nodes);
  for (int i = 0; i < size; i++) {
    string line = cJSON_GetArrayItem(nodes, i)->valuestring;
    items->push_back(line);
  }

  cJSON_Delete(root);

  return index;
}

bool ReduceRecord::fromJson(std::string* json) {
  cJSON* root = cJSON_Parse(json->c_str());
  if (root == NULL) {
    return false;
  }

  auto node_did = cJSON_GetObjectItem(root, "dId");
  if (node_did == NULL) {
    return false;
  }
  this->did = node_did->valuestring;

  // auto ts_did = cJSON_GetObjectItem(root, "ts");
  // if (ts_did == NULL) {
  //   return false;
  // }
  // this->ts = ts_did->valueint;  // 日志数据时间戳不对

  cJSON_Delete(root);

  return true;
}

std::string ServicePaths::kafka_start = "dsc.kafka.start";
std::string ServicePaths::map_start = "dsc.map.start";
std::string ServicePaths::map_invoke = "dsc.map.invoke";
std::string ServicePaths::reduce_start = "dsc.reduce.start";
std::string ServicePaths::reduce_innvoke = "dsc.reduce.innvoke";