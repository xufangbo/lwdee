#include "ReduceDCO.h"
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
    logger_error("step2 failed,%s,%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
    return "failed";
  } catch (std::exception& ex) {
    logger_error("step2 failed,%s", ex.what());
    return "failed";
  }
}

std::string ReduceDCO::reduce(std::string a) {
  try {
    Words words;
    int mapIndex = StringsSerializer::fromJson(a, words);

    logger_debug("accept reduce, %d words, (map-%02d, reduce-%02d)",words.size(),mapIndex,input.index);

    this->reducer.accept(mapIndex,words);

    return "success";
  } catch (Exception& ex) {
    logger_error("step2 failed,%s,%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
    return "failed";
  } catch (std::exception& ex) {
    logger_error("step2 failed,%s", ex.what());
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
