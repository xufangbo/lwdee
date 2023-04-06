#include "Partition.h"
#include "core/cjson.hpp"

std::string PartitionKafka::toJson() {
  cJSON* root = cJSON_CreateObject();

  cJSON_AddNumberToObject(root, "index", index);
  cJSON_AddStringToObject(root, "group", group.c_str());
  cJSON_AddStringToObject(root, "topic", topic.c_str());

  char* jsonText = cJSON_Print(root);

  return jsonText;
}

void PartitionKafka::fromJson(std::string* json) {
  cJSON* node = cJSON_Parse(json->c_str());
  index = cJSON_GetObjectItem(node, "index")->valueint;
  group = cJSON_GetObjectItem(node, "group")->valuestring;
  topic = cJSON_GetObjectItem(node, "topic")->valuestring;
}

std::string PartitionMap::toJson() {
  cJSON* root = cJSON_CreateObject();

  cJSON_AddNumberToObject(root, "index", index);
  cJSON_AddStringToObject(root, "fileName", fileName.c_str());

  char* jsonText = cJSON_Print(root);

  return jsonText;
}

void PartitionMap::fromJson(std::string* json) {
  cJSON* node = cJSON_Parse(json->c_str());
  index = cJSON_GetObjectItem(node, "index")->valueint;
  fileName = cJSON_GetObjectItem(node, "fileName")->valuestring;
}

std::string PartitionReduce::toJson() {
  cJSON* root = cJSON_CreateObject();

  cJSON_AddNumberToObject(root, "index", index);
  cJSON_AddStringToObject(root, "fileName", outputFile.c_str());

  cJSON* nodes = cJSON_CreateArray();
  cJSON_AddItemToObject(root, "subSplitDDOs", nodes);

  for (SubSplitDDO& item : subSplitDDOs) {
    cJSON* split = cJSON_CreateObject();

    cJSON_AddStringToObject(split, "voxorId", item.voxorId.c_str());
    cJSON_AddStringToObject(split, "dataId", std::to_string(item.dataId).c_str());

    cJSON_AddItemToArray(nodes, split);
  }

  char* jsonText = cJSON_Print(root);

  return jsonText;
}

void PartitionReduce::fromJson(std::string* json) {
  cJSON* node = cJSON_Parse(json->c_str());
  index = cJSON_GetObjectItem(node, "index")->valueint;
  outputFile = cJSON_GetObjectItem(node, "fileName")->valuestring;

  cJSON* subNodes = cJSON_GetObjectItem(node, "subSplitDDOs");
  cJSON* child = subNodes->child;
  while (child != NULL) {
    SubSplitDDO split;
    split.voxorId = cJSON_GetObjectItem(child, "voxorId")->valuestring;

    char* dataId = cJSON_GetObjectItem(child, "dataId")->valuestring;
    split.dataId = strtoull(dataId, NULL, 0);

    this->subSplitDDOs.push_back(split);

    child = child->next;
  }
}

std::string Step2Output::toJson() {
  cJSON* root = cJSON_CreateObject();

  // cJSON_AddNumberToObject(root, "index", index);
  // cJSON_AddStringToObject(root, "fileName", outputFile.c_str());

  // cJSON* nodes = cJSON_CreateArray();
  // cJSON_AddItemToObject(root, "subSplitDDOs", nodes);

  // for (SubSplitDDO& item : subSplitDDOs) {
  //   cJSON* split = cJSON_CreateObject();

  //   cJSON_AddStringToObject(split, "voxorId", item.voxorId.c_str());
  //   cJSON_AddStringToObject(split, "dataId", std::to_string(item.dataId).c_str());

  //   cJSON_AddItemToArray(nodes, split);
  // }

  char* jsonText = cJSON_Print(root);

  return jsonText;
}

void Step2Output::fromJson(std::string* json) {
  cJSON* node = cJSON_Parse(json->c_str());

  // index = cJSON_GetObjectItem(node, "index")->valueint;
  // outputFile = cJSON_GetObjectItem(node, "fileName")->valuestring;

  // cJSON* subNodes = cJSON_GetObjectItem(node, "subSplitDDOs");
  // cJSON* child = subNodes->child;
  // while (child != NULL) {
  //   SubSplitDDO split;
  //   split.voxorId = cJSON_GetObjectItem(child, "voxorId")->valuestring;

  //   char* dataId = cJSON_GetObjectItem(child, "dataId")->valuestring;
  //   split.dataId = strtoull(dataId, NULL, 0);

  //   this->subSplitDDOs.push_back(split);

  //   child = child->next;
  // }
}