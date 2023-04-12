#include "ReduceDCO.h"
#include <memory>
#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/Stopwatch.h"
#include "core/log.hpp"
#include "matrix/LinuxMatrix.h"

std::string ReduceDCO::start(std::string a) {
  try {
    logger_info("reduce start");

    Stopwatch sw;
    LinuxMatrix::print();
    input.fromJson(&a);
    // LinuxMatrix::print();

    return "success";
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
    Stopwatch sw;
    // std::shared_ptr<Words> words = std::make_shared<Words>();
    // int mapIndex = StringsSerializer::fromJson(a, words.get());
    // this->reducer.accept(mapIndex, words);

    // logger_debug("accept reduce, %d words, (map-%02d, reduce-%02d),eclapse:%lfs", words->size(), mapIndex, input.index,sw.stop());
    logger_debug("accept reduce");

    return "success";
  } catch (Exception& ex) {
    logger_error("reduce failed,%s,%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
    return "failed";
  } catch (std::exception& ex) {
    logger_error("reduce failed,%s", ex.what());
    return "failed";
  }
}

ReduceDCO::ReduceDCO() {
  getFunctionTable()["reduce"] = (PTR)&ReduceDCO::reduce;
  getFunctionTable()["start"] = (PTR)&ReduceDCO::start;
  name = "ReduceDCO";
}
ReduceDCO::~ReduceDCO() {
}
