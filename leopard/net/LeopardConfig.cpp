#include "LeopardConfig.hpp"
#include "core/cjson.hpp"
#include "core/log.hpp"

LeopardConfig LeopardConfig::_instance;

void LeopardConfig::readConfig() {
  if (_instance.port > 0) {
    return;
  }
  std::string configFile = "./leopard.json";
  FILE* fp = fopen(configFile.c_str(), "r");
  if (fp != NULL) {
    fclose(fp);
  } else {
    configFile = "./config/leopard.json";
    fp = fopen(configFile.c_str(), "r");
    if (fp != NULL) {
      fclose(fp);
    } else {
      configFile = "/home/ubuntu/gitee/leopard/config/leopard.json";
    }
  }

  std::string json;

  fp = fopen(configFile.c_str(), "r");
  if (fp == NULL) {
    logger_error("can't open leopard.json, %s", configFile.c_str());
  }
  char c = 0;
  while ((c = fgetc(fp)) != EOF) {
    json += c;
  }

  fclose(fp);

  cJSON* node = cJSON_Parse(json.c_str());
  _instance.ip = cJSON_GetObjectItem(node, "ip")->valuestring;
  _instance.port = cJSON_GetObjectItem(node, "port")->valueint;
  _instance.procnums = cJSON_GetObjectItem(node, "procnums")->valueint;

  cJSON_Delete(node);

  // logger_debug("%s:%d %d", ip.c_str(),  _instance.port, _instance.procnums);
}

LeopardConfig* LeopardConfig::instance() {
  return &_instance;
}