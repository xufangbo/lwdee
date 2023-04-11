#include "MapDCO.h"

#include <iostream>
#include <sstream>

#include "Mapper.h"
#include "core/DscConfig.hpp"
#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/Stopwatch.h"
#include "core/UhconnVoxorFactory.h"
#include "core/UhconnWorkNode.h"
#include "core/cjson.hpp"
#include "core/log.hpp"
#include "matrix/LinuxMatrix.h"

std::string MapDCO::start(std::string a) {
  try {
    // std::vector<std::string> lines;

    // cJSON* node = cJSON_Parse(a.c_str());
    // int size = cJSON_GetArraySize(node);
    // for (int i = 0; i < size; i++) {
    // std:
    //   string line = cJSON_GetArrayItem(node, i)->valuestring;
    //   lines.push_back(line);
    // }

    // logger_debug("map dco accept %d ", lines.size());

    // vector<string> words;
    // Mapper::map(lines, words);

    // toReduce.send(words);

    return "succeed";

  } catch (Exception& ex) {
    logger_error("step2 failed,%s,%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
    return "failed";
  } catch (std::exception& ex) {
    logger_error("step2 failed,%s", ex.what());
    return "failed";
  }
}

std::string MapDCO::map(std::string a) {
  try {
    std::vector<std::string> lines;

    cJSON* node = cJSON_Parse(a.c_str());
    int size = cJSON_GetArraySize(node);
    for (int i = 0; i < size; i++) {
    std:
      string line = cJSON_GetArrayItem(node, i)->valuestring;
      lines.push_back(line);
    }

    logger_debug("map dco accept %d ", lines.size());

    vector<string> words;
    Mapper::map(lines, words);

    toReduce.send(words);

    return "succeed";

  } catch (Exception& ex) {
    logger_error("step2 failed,%s,%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
    return "failed";
  } catch (std::exception& ex) {
    logger_error("step2 failed,%s", ex.what());
    return "failed";
  }
}

MapDCO::MapDCO() {
  getFunctionTable()["start"] = (PTR)&MapDCO::start;
  getFunctionTable()["map"] = (PTR)&MapDCO::map;
  toReduce.create_dcos();
  name = "MapDCO";
}
MapDCO::~MapDCO() {}
