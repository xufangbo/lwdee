#include "ReduceDCO.h"
#include <fstream>
#include <memory>
#include "core/DscConfig.hpp"
#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "matrix/LinuxMatrix.h"
#include "net/BufferStream.hpp"
#include "server/TcpResponse.hpp"

void createFile(int index) {
  auto conf = DscConfig::instance();
  std::string fileName = conf->outputFile + conf->nodeName + "_reduce_" + std::to_string(index) + ".csv";

  // if (access(fileName.c_str(), 0) == 0) {
  //   if (remove(fileName.c_str()) == 0) {
  //     logger_trace("rm %s", fileName.c_str());
  //   } else {
  //     logger_error("can't rm %s", fileName.c_str());
  //   }
  // }

  std::ofstream f(fileName, std::ios_base::trunc);
  if (!f.is_open()) {
    logger_trace("can't open file %s", fileName.c_str());
  }

 f << "time" << ",";

  f << "index" << ",";
  f << "c_input" << ",";
  f << "c_output" << ",";
  f << "b_delay" << ",";
  f << "b_sum" << ",";
  f << "b_size" << ",";
  f << "t_delay" << ",";
  f << "t_sum" << ",";
  f << "t_size" << ",";

  f << std::endl;

  f.flush();
  f.close();
}

std::string ReduceDCO::start(std::string a) {
  printf("\n");
  try {
    LinuxMatrix::start();

    logger_info("reduce start");

    Stopwatch sw;
    // LinuxMatrix::print();
    input.fromJson(&a);
    createFile(input.index);
    // LinuxMatrix::print();

    LinuxMatrix::stream.reduce_dco++;

    return "succeed";
  } catch (Exception& ex) {
    logger_error("reduce start failed,%s,%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
    return "failed";
  } catch (std::exception& ex) {
    logger_error("reduce start failed,%s", ex.what());
    return "failed";
  }
}

std::string ReduceDCO::reduce(std::string a) {
  try {
    // logger_debug("< accept reduce ");
    Stopwatch sw;

    auto records = std::make_shared<std::vector<ReduceRecord>>();

    ReduceInvokeData reduceInvokeData(0, records.get());
    reduceInvokeData.fromJson(&a);

    LinuxMatrix::stream.reduce_accept += records->size();
    reducer.accept(records.get(), &input);

    // logger_debug("accept reduce, %d records, (map-%02d, reduce-%02d),eclapse:%lfs", records->size(), reduceInvokeData.mapIndex, input.index, sw.stop());
    // logger_debug("accept reduce");

    records->clear();

    // LinuxMatrix::print();

    return "success";
  } catch (Exception& ex) {
    logger_error("reduce failed,%s,%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
    return "failed";
  } catch (std::exception& ex) {
    logger_error("reduce failed,%s", ex.what());
    return "failed";
  }
}



void regist_reduce_start_service() {
  TcpResponse::regist(ServicePaths::map_start, [](BufferStream* inputStream,
                                                  BufferStream* outputStream) {
    // 1. inputStream -> order request parameter
    auto len = inputStream->get<uint32_t>();
    auto content = inputStream->getString(len);

    ReduceDCO::start(content);

    // 3. outputStream
    std::string message = "succeed";
    outputStream->put<uint32_t>(message.size());
    outputStream->put(message);
  });
}

void regist_reduce_invoke_service() {
  TcpResponse::regist(ServicePaths::map_invoke, [](BufferStream* inputStream,
                                                   BufferStream* outputStream) {
    // 1. inputStream -> order request parameter
    auto len = inputStream->get<uint32_t>();
    auto content = inputStream->getString(len);

    ReduceDCO::reduce(content);

    // 3. outputStream
    std::string message = "succeed";
    outputStream->put<uint32_t>(message.size());
    outputStream->put(message);
  });
}