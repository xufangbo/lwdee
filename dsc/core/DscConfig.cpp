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
  inputFile = cJSON_GetObjectItem(node, "inputFile")->valuestring;
  brokers = cJSON_GetObjectItem(node, "brokers")->valuestring;
  topic = cJSON_GetObjectItem(node, "topic")->valuestring;
  splitNums1 = cJSON_GetObjectItem(node, "splitNums1")->valueint;
  splitNums2 = cJSON_GetObjectItem(node, "splitNums2")->valueint;

  cJSON* partitionsNode = cJSON_GetObjectItem(node, "partitions");
  int size = cJSON_GetArraySize(partitionsNode);
  for (int i = 0; i < size; i++) {
    int partitionId = cJSON_GetArrayItem(partitionsNode, i)->valueint;
    partitions.push_back(partitionId);
  }


  std::string partionsString = "[";
  for (int i = 0; i < size - 1; i++) {
    partionsString += std::to_string(partitions[i]);
    partionsString += ",";
  }
  partionsString += std::to_string(partitions[size - 1]);
  partionsString += "]";
  logger_debug("%s %s %s %s %d %d", inputFile.c_str(), brokers.c_str(),topic.c_str(), partitionsNode->string, _instance.splitNums1, _instance.splitNums2);
  
}

DscConfig* DscConfig::instance() {
  return &_instance;
}