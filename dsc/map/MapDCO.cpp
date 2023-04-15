#include "MapDCO.h"

#include <iostream>
#include <sstream>

#include "Mapper.h"
#include "core/DscConfig.hpp"
#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/Stopwatch.hpp"
#include "core/cjson.hpp"
#include "core/log.hpp"
#include "matrix/LinuxMatrix.h"

int MapDCO::start(std::string a) {
  printf("\n");
  try {
    LinuxMatrix::start();

    logger_trace("accept map sart");
    Stopwatch sw;

    input.fromJson(&a);
    toReduce.create_dcos(&input);

    logger_trace("> accept map sart,partition : %d,eclipse %lf", input.index,sw.stop());
    LinuxMatrix::stream.map_dco++;
    // LinuxMatrix::print();

    return input.index;

  } catch (Exception& ex) {
    logger_error("step2 failed,%s,%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
    return -1;
  } catch (std::exception& ex) {
    logger_error("step2 failed,%s", ex.what());
    return-1;
  }
}

std::string MapDCO::map(std::shared_ptr<vector<MapRecord>> lines) {
  try {
    // logger_trace("< accept map,%s",a.c_str());
    Stopwatch sw;
   

    LinuxMatrix::stream.map_accept += lines->size();
    // LinuxMatrix::print();

    auto words = std::make_shared<vector<ReduceRecord>>();
    Mapper::map(lines.get(), words.get());

    toReduce.send(words.get());

    // logger_trace("accept map, %d lines, (kafka-%02d,map-%02d),eclapse:%lfs",
    // lines->size(), mapInvokeDta.kafkaIndex, input.index, sw.stop());

    lines->clear();
    words->clear();

    return "succeed";

  } catch (Exception& ex) {
    logger_error("map failed,%s,%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
    return "failed";
  } catch (std::exception& ex) {
    logger_error("map failed,%s", ex.what());
    return "failed";
  }
}
