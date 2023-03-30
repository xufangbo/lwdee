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

  char c = 0;
  while ((c = fgetc(fp)) != EOF) {
    json += c;
  }

  fclose(fp);

  cJSON* node = cJSON_Parse(json.c_str());
  _instance.inputFile = cJSON_GetObjectItem(node, "inputFile")->valueint;
  _instance.outputFile = cJSON_GetObjectItem(node, "outputFile")->valuestring;
  _instance.datum = cJSON_GetObjectItem(node, "datum")->valueint;
  _instance.splitNums1 = cJSON_GetObjectItem(node, "splitNums1")->valueint;
  _instance.splitNums2 = cJSON_GetObjectItem(node, "splitNums2")->valueint;

  logger_debug("%s %s", inputFile.c_str(), outputFile.c_str());
}

TerasortConfig* TerasortConfig::instance() {
  return &_instance;
}