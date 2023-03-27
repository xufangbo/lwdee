#include "Partition.h"

std::string Step1Output::toJson() {
  cJSON* root = cJSON_CreateObject();

  cJSON* nodes = cJSON_CreateArray();
  cJSON_AddItemToObject(root, "items", nodes);

  for (Step1OutputItem& item : items) {
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
    Step1OutputItem split;
    split.voxorId = cJSON_GetObjectItem(child, "voxorId")->valuestring;

    char* dataId = cJSON_GetObjectItem(child, "dataId")->valuestring;
    split.dataId = strtoull(dataId, NULL, 0);

    this->items.push_back(split);

    child = child->next;
  }
}