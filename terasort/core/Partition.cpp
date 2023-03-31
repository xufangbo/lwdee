#include "Partition.h"
#include "core/cjson.hpp"

std::string PartitionStep0::toJson() {
  cJSON* root = cJSON_CreateObject();

  cJSON_AddNumberToObject(root, "index", index);
  cJSON_AddNumberToObject(root, "splitNums1", splitNums1);
  cJSON_AddNumberToObject(root, "datum", datum);
  cJSON_AddStringToObject(root, "fileName", fileName.c_str());

  char* jsonText = cJSON_Print(root);

  return jsonText;
}

void PartitionStep0::fromJson(std::string* json) {
  cJSON* node = cJSON_Parse(json->c_str());
  index = cJSON_GetObjectItem(node, "index")->valueint;
  splitNums1 = cJSON_GetObjectItem(node, "splitNums1")->valueint;
  datum = cJSON_GetObjectItem(node, "datum")->valueint;
  fileName = cJSON_GetObjectItem(node, "fileName")->valuestring;
}

std::string PartitionStep1::toJson() {
  cJSON* root = cJSON_CreateObject();

  cJSON_AddNumberToObject(root, "index", index);
  cJSON_AddStringToObject(root, "fileName", fileName.c_str());

  cJSON* nodes = cJSON_CreateArray();
  cJSON_AddItemToObject(root, "sampleSplits", nodes);

  for (SampleSplit& item : sampleSplits) {
    cJSON* split = cJSON_CreateObject();
    cJSON_AddStringToObject(split, "min", std::to_string(item.min).c_str());
    cJSON_AddStringToObject(split, "max", std::to_string(item.max).c_str());

    cJSON_AddItemToArray(nodes, split);
  }

  char* jsonText = cJSON_Print(root);

  return jsonText;
}

void PartitionStep1::fromJson(std::string* json) {
  cJSON* node = cJSON_Parse(json->c_str());
  index = cJSON_GetObjectItem(node, "index")->valueint;
  fileName = cJSON_GetObjectItem(node, "fileName")->valuestring;

  cJSON* subNodes = cJSON_GetObjectItem(node, "sampleSplits");
  cJSON* child = subNodes->child;
  while (child != NULL) {
    SampleSplit split;
    char* min = cJSON_GetObjectItem(child, "min")->valuestring;
    split.min = strtoull(min, NULL, 0);

    char* max = cJSON_GetObjectItem(child, "max")->valuestring;
    split.max = strtoull(max, NULL, 0);

    sampleSplits.push_back(split);

    child = child->next;
  }
}

std::string Step1Output::toJson() {
  cJSON* root = cJSON_CreateObject();

  cJSON* nodes = cJSON_CreateArray();
  cJSON_AddItemToObject(root, "items", nodes);

  for (SubSplitDDO& item : items) {
    cJSON* split = cJSON_CreateObject();

    cJSON_AddStringToObject(split, "voxorId", item.voxorId.c_str());
    cJSON_AddStringToObject(split, "dataId", std::to_string(item.dataId).c_str());

    cJSON_AddItemToArray(nodes, split);
  }

  char* jsonText = cJSON_Print(root);

  return jsonText;
}

void Step1Output::fromJson(std::string* json) {
  cJSON* node = cJSON_Parse(json->c_str());

  cJSON* subNodes = cJSON_GetObjectItem(node, "items");
  cJSON* child = subNodes->child;
  while (child != NULL) {
    SubSplitDDO split;
    split.voxorId = cJSON_GetObjectItem(child, "voxorId")->valuestring;

    char* dataId = cJSON_GetObjectItem(child, "dataId")->valuestring;
    split.dataId = strtoull(dataId, NULL, 0);

    this->items.push_back(split);

    child = child->next;
  }
}

std::string PartitionStep2::toJson() {
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

void PartitionStep2::fromJson(std::string* json) {
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