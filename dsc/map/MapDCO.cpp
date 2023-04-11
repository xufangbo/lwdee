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
    // logger_info("< invokded start %s", a.c_str());

    input.fromJson(&a);
    toReduce.create_dcos(&input);

    logger_trace("> accept map sart,partition : %d,eclipse %lf", input.index, sw.stop());

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
    std::vector<std::string> lines;

    StringsSerializer::fromJson(a, lines);

    logger_debug("accept map, %d lines,partition: %d ", lines.size(), input.index);

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
  name = "MapDCO";
}
MapDCO::~MapDCO() {}
