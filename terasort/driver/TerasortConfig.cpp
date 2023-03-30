#include "TerasortConfig.hpp"
#include "core/cjson.hpp"
#include "core/log.hpp"

TerasortConfig TerasortConfig::_instance;

void TerasortConfig::readConfig() {
  if (_instance.splitNums1 > 0) {
    return;
  }
  std::string configFile = "./terasort.json";
  FILE* fp = fopen(configFile.c_str(), "r");
  if (fp != NULL) {
    fclose(fp);
  } else {
    configFile = "./config/terasort.json";
    fp = fopen(configFile.c_str(), "r");
    if (fp != NULL) {
      fclose(fp);
    } else {
      configFile = "/home/kevin/git/lwdee/config/terasort.json";
    }
  }

  std::string json;

  fp = fopen(configFile.c_str(), "r");
  if (fp == NULL) {
    logger_error("can't open terasort.json, %s", configFile.c_str());
  }
  char c = 0;
  while ((c = fgetc(fp)) != EOF) {
    json += c;
  }

  fclose(fp);

  cJSON* node = cJSON_Parse(json.c_str());
  _instance.inputFile = cJSON_GetObjectItem(node, "inputFile")->valuestring;
  _instance.outputFile = cJSON_GetObjectItem(node, "outputFile")->valuestring;
  _instance.datum = cJSON_GetObjectItem(node, "datum")->valueint;
  _instance.splitNums1 = cJSON_GetObjectItem(node, "splitNums1")->valueint;
  _instance.splitNums2 = cJSON_GetObjectItem(node, "splitNums2")->valueint;
  _instance.is_matrix = cJSON_GetObjectItem(node, "is_matrix")->valueint;

  logger_debug("%s %s %d %d %d", inputFile.c_str(), outputFile.c_str(), _instance.datum, _instance.splitNums1, _instance.splitNums2);
}

TerasortConfig* TerasortConfig::instance() {
  return &_instance;
}