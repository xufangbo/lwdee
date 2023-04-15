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
#include "net/BufferStream.hpp"
#include "server/TcpResponse.hpp"

std::string MapDCO::start(std::string a) {
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

    return "succeed";

  } catch (Exception& ex) {
    logger_error("step2 failed,%s,%s", ex.getMessage().c_str(),
                 ex.getStackTrace().c_str());
    return "failed";
  } catch (std::exception& ex) {
    logger_error("step2 failed,%s", ex.what());
    return "failed";
  }
}

std::string MapDCO::map(std::string a) {
  try {
    // logger_trace("< accept map,%s",a.c_str());
    Stopwatch sw;
    auto lines = std::make_shared<vector<MapRecord>>();
    MapInvokeData mapInvokeDta(0, lines.get());
    mapInvokeDta.fromJson(&a);

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
    logger_error("map failed,%s,%s", ex.getMessage().c_str(),
                 ex.getStackTrace().c_str());
    return "failed";
  } catch (std::exception& ex) {
    logger_error("map failed,%s", ex.what());
    return "failed";
  }
}

void regist_map_start_service() {
  TcpResponse::regist(ServicePaths::map_start, [](BufferStream* inputStream,
                                                  BufferStream* outputStream) {
    // 1. inputStream -> order request parameter
    auto len = inputStream->get<uint32_t>();
    auto content = inputStream->getString(len);

    MapDCO::start(content);

    // 3. outputStream
    std::string message = "succeed";
    outputStream->put<uint32_t>(message.size());
    outputStream->put(message);
  });
}

void regist_map_invoke_service() {
  TcpResponse::regist(ServicePaths::map_invoke, [](BufferStream* inputStream,
                                                   BufferStream* outputStream) {
    // 1. inputStream -> order request parameter
    auto len = inputStream->get<uint32_t>();
    auto content = inputStream->getString(len);

    MapDCO::map(content);

    // 3. outputStream
    std::string message = "succeed";
    outputStream->put<uint32_t>(message.size());
    outputStream->put(message);
  });
}