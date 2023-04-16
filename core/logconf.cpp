#include <stdio.h>
#include <string.h>
#include <iostream>
#include "cjson.hpp"
#include "log.hpp"

void read_log_config(const char* name) {
  char* configFile = "./log.json";
  FILE* fp = fopen(configFile, "r");
  if (fp != NULL) {
    fclose(fp);
  } else {
    configFile = "./config/log.json";
    fp = fopen(configFile, "r");
    if (fp != NULL) {
      fclose(fp);
    } else {
      configFile = "/home/kevin/git/lwdee/config/log.json";
    }
  }

  std::string json;

  fp = fopen(configFile, "r");
  if (fp == NULL) {
    logger_error("can't open log.json, %s", configFile);
  }
  char c = 0;
  while ((c = fgetc(fp)) != EOF) {
    json += c;
  }

  fclose(fp);

  LogOption option;
  option.initalized = false;
  option.level = log_trace;

  cJSON* root = cJSON_Parse(json.c_str());
  option.level = (LogLevel)cJSON_GetObjectItem(root, "level")->valueint;
  option.is_color = cJSON_GetObjectItem(root, "color")->valueint;
  option.days = cJSON_GetObjectItem(root, "days")->valueint;
  option.writeFile = cJSON_GetObjectItem(root, "writeFile")->valueint;

  char* fileMode = cJSON_GetObjectItem(root, "fileMode")->valuestring;
  char* path = cJSON_GetObjectItem(root, "path")->valuestring;

  strcpy(option.fileMode, "w+");
  strcpy(option.path, "./log");
  strcpy(option.name, name);

  cJSON_Delete(root);

  if (logger_initialize(option) != 0) {
    printf("log initialize error\n");
  } else {
    logger_info("-- app starting ... ");
  }
}