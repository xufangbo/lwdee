#include "DscConfig.hpp"
#include "core/cjson.hpp"
#include "core/log.hpp"

DscConfig DscConfig::_instance;

void DscConfig::readConfig() {
  if (_instance.splitNums1 > 0) {
    return;
  }
  std::string configFile = "./dsc.json";
  FILE* fp = fopen(configFile.c_str(), "r");
  if (fp != NULL) {
    fclose(fp);
  } else {
    configFile = "./config/dsc.json";
    fp = fopen(configFile.c_str(), "r");
    if (fp != NULL) {
      fclose(fp);
    } else {
      configFile = "/home/kevin/git/lwdee/config/dsc.json";
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

  cJSON* node = cJSON_Parse(json.c_str());
  outputFile = cJSON_GetObjectItem(node, "outputFile")->valuestring;
  inputFile = cJSON_GetObjectItem(node, "inputFile")->valuestring;
  name = cJSON_GetObjectItem(node, "name")->valuestring;
  brokers = cJSON_GetObjectItem(node, "brokers")->valuestring;
  group = cJSON_GetObjectItem(node, "group")->valuestring;
  topic = cJSON_GetObjectItem(node, "topic")->valuestring;
  outTopic = cJSON_GetObjectItem(node, "outTopic")->valuestring;
  window = cJSON_GetObjectItem(node, "window")->valueint;
  inputFilter = cJSON_GetObjectItem(node, "inputFilter")->valueint;
  splitNums1 = cJSON_GetObjectItem(node, "splitNums1")->valueint;
  splitNums2 = cJSON_GetObjectItem(node, "splitNums2")->valueint;

  cJSON* partitionsNode = cJSON_GetObjectItem(node, "partitions");
  int size = cJSON_GetArraySize(partitionsNode);
  for (int i = 0; i < size; i++) {
    int partitionId = cJSON_GetArrayItem(partitionsNode, i)->valueint;
    partitions.push_back(partitionId);
  }

  logger_debug("%s %s %s %d %d",  brokers.c_str(), topic.c_str(), partitionsString().c_str(), _instance.splitNums1, _instance.splitNums2);
}

std::string DscConfig::partitionsString() {
  std::string str = "[";
  for (int i = 0; i < partitions.size() - 1; i++) {
    str += std::to_string(partitions[i]);
    str += ",";
  }
  str += std::to_string(partitions[partitions.size() - 1]);
  str += "]";
  return str;
}

DscConfig* DscConfig::instance() {
  return &_instance;
}