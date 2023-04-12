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
    logger_trace("accept map sart");
    Stopwatch sw;

    input.fromJson(&a);
    toReduce.create_dcos(&input);

    logger_trace("> accept map sart,partition : %d,eclipse %lf", input.index, sw.stop());
    LinuxMatrix::print();

    return "succeed!";

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
    logger_trace("< accept map");
    Stopwatch sw;
    auto lines = std::make_shared<vector<string>>();
    int kafkaIndex = StringsSerializer::fromJson(a, lines.get());

    LinuxMatrix::print();

    auto words = std::make_shared<vector<string>>();
    Mapper::map(lines.get(), words.get());

    toReduce.send(words.get());

    logger_trace("> accept map, %d lines, (kafka-%02d,map-%02d),eclapse:%lfs", lines->size(), kafkaIndex, input.index, sw.stop());
    return "succeed";

  } catch (Exception& ex) {
    logger_error("map failed,%s,%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
    return "failed";
  } catch (std::exception& ex) {
    logger_error("map failed,%s", ex.what());
    return "failed";
  }
}

MapDCO::MapDCO() {
  getFunctionTable()["start"] = (PTR)&MapDCO::start;
  getFunctionTable()["map"] = (PTR)&MapDCO::map;
  name = "MapDCO";
}
MapDCO::~MapDCO() {}
