#include "Driver.h"

#include <algorithm>
#include <fstream>
#include <vector>

#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/Stopwatch.h"
#include "core/log.hpp"
#include "lwdee/DDO.h"
#include "lwdee/lwdee.h"
#include "map/Step1Task.h"
#include "matrix/LinuxMatrix.h"

void Driver::startJob() {
  logger_info("> job");
  LinuxMatrix::print();

  this->conf = DscConfig::instance();

  try {
    Stopwatch sw;

    this->map();
    this->mapToReduce();
    this->reduce();

    LinuxMatrix::print();
    logger_info("< job,eclipse %lf s", sw.stop());

    logger_info("finished");
  } catch (Exception& ex) {
    logger_error("execute job error,%s", ex.getMessage().c_str());
  }
}

void Driver::map() {
  logger_info("< map");
  Stopwatch sw;

  for (int i = 0; i < conf->splitNums1; i++) {
    logger_debug("invoke dco.map, split: %d", (i + 1));

    DCO dco = lwdee::create_dco("MapDCO");
    // DCO dco = lwdee::create_dco(i + 2, "MapDCO");

    PartitionStep1 input(i, conf->inputFile);
    auto json = input.toJson();

    DDOId ddoId = dco.async("start", json);

    step1Invokers.push_back(std::make_pair(dco, ddoId));
  }

  logger_info("> map,eclipse %lf", sw.stop());
}

void Driver::mapToReduce() {
  logger_info("< mapToReduce");
  Stopwatch sw;

  std::vector<Step1Output> step1Outputs;
  for (auto& kv : step1Invokers) {
    DCO dco = kv.first;
    DDOId ddoId = kv.second;

    try {
      auto ddo = dco.wait(ddoId);
      auto bytes = ddo.read();

      // logger_debug("get step 1 ddo(%ld),%s", ddo.ddoId.itsId(), bytes->c_str());

      Step1Output setp1Output;
      setp1Output.fromJson(bytes.get());

      ddo.releaseGlobal();

      step1Outputs.push_back(setp1Output);
    } catch (Exception& ex) {
      ex.trace(ZONE);
    }
  }

  for (int i = 0; i < conf->splitNums2; i++) {
    PartitionStep2 step2Input(i, conf->outTopic);
    for (Step1Output& step1Output : step1Outputs) {
      step2Input.subSplitDDOs.push_back(step1Output.items[i]);
    }

    step2Inputs.push_back(step2Input);
  }

  logger_info("> mapToReduce,eclipse %lf", sw.stop());
}

void Driver::reduce() {
  logger_info("< reduce");
  Stopwatch sw;

  for (int i = 0; i < step2Inputs.size(); i++) {
    logger_debug("invoke dco.reduce, split: %d", (i + 1));

    PartitionStep2& step2Input = step2Inputs[i];

    DCO dco = lwdee::create_dco("ReduceDCO");
    // DCO dco = lwdee::create_dco(i + 2, "ReduceDCO");
    auto json = step2Input.toJson();

    DDOId ddoId = dco.async("reduce", json);

    step2Invokers.push_back(std::make_pair(dco, ddoId));
  }

  for (auto& kv : step2Invokers) {
    DCO dco = kv.first;
    DDOId ddoId = kv.second;

    try {
      auto ddo = dco.wait(ddoId);
      auto bytes = ddo.read();

      logger_debug("get step 2 ddo(node%d),%s", ddo.ddoId.itsWorkNodeId(), bytes->c_str());

      // Step1Output setp1Output;
      // setp1Output.fromJson(bytes.get());

      ddo.releaseGlobal();
    } catch (Exception& ex) {
      ex.trace(ZONE);
    }
  }

  logger_info("> reduce,eclipse %lf", sw.stop());
}