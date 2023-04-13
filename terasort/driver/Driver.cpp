#include "Driver.h"

#include <algorithm>
#include <fstream>
#include <vector>

#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/Stopwatch.h"
#include "core/log.hpp"
#include "driver/TerasortConfig.hpp"
#include "lwdee/DDO.h"
#include "lwdee/lwdee.h"
#include "map/Step1Task.h"
#include "matrix/LinuxMatrix.h"

void Driver::startJob() {
  logger_info("> job");
  // LinuxMatrix::print();

  this->inputFile = TerasortConfig::instance()->inputFile;
  this->outputFile = TerasortConfig::instance()->outputFile;
  this->datum = TerasortConfig::instance()->datum;
  this->splitNums1 = TerasortConfig::instance()->splitNums1;
  this->splitNums2 = TerasortConfig::instance()->splitNums2;

  try {
    Stopwatch sw;

    auto conf = this->samples(inputFile);
    this->split(conf);
    this->map();
    this->mapToReduce();
    this->reduce();

    // LinuxMatrix::print();
    logger_info("< job,eclipse %lf s", sw.stop());

    logger_info("finished");
  } catch (Exception& ex) {
    logger_error("execute job error,%s", ex.getMessage().c_str());
  }
}

std::pair<uint64_t, uint64_t> Driver::samples(std::string fileName) {
  logger_info("< samples");
  Stopwatch sw;

  // 执行分布式调用
  std::vector<pair<DCO, DDOId>> step0Invokers;
  for (int i = 0; i < splitNums1; i++) {
    logger_debug("invoke dco.sample, split: %d", (i + 1));

    DCO dco = lwdee::create_dco("SampleDCO");
    // DCO dco = lwdee::create_dco(i + 2, "SampleDCO");

    PartitionStep0 input(i, inputFile, splitNums1, datum);
    auto json = input.toJson();

    DDOId ddoId = dco.async("sample", json);

    step0Invokers.push_back(std::make_pair(dco, ddoId));
  }

  // 获取调用结果DDO

  std::vector<uint64_t> mins;
  std::vector<uint64_t> maxs;
  for (auto& kv : step0Invokers) {
    DCO dco = kv.first;
    DDOId ddoId = kv.second;

    try {
      auto ddo = dco.wait(ddoId);
      auto bytes = ddo.read();

      int index = bytes->find('-');
      auto min = bytes->substr(0, index);
      auto max = bytes->substr(index + 1, bytes->size() - index - 1);

      // logger_debug("sample rc : %s,min: %s,max: %s", bytes->c_str(), min.c_str(), max.c_str());

      ddo.releaseGlobal();

      mins.push_back(strtoull(min.c_str(), NULL, 0));
      maxs.push_back(strtoull(max.c_str(), NULL, 0));

    } catch (Exception& ex) {
      ex.trace(ZONE);
    }
  }

  uint64_t min = *std::min_element(mins.begin(), mins.end());
  uint64_t max = *std::max_element(maxs.begin(), maxs.end());

  auto pair = std::make_pair(min, max);

  logger_info("> samples,%llu-%llu,eclipse %lf", min, max, sw.stop());

  return pair;
}

void Driver::split(std::pair<uint64_t, uint64_t> conf) {
  auto seprator = (conf.second - conf.first) / splitNums2;

  uint64_t pre = 0;
  for (int i = 0; i < splitNums2; i++) {
    SampleSplit split;
    split.min = pre;
    split.max = conf.first + seprator * (i + 1);

    pre = split.max;
    if (i + 1 == splitNums2) {
      split.max = 0;
    }

    sampleSplits.push_back(split);

    logger_trace("subsplit %d : %llu - %llu", i, split.min, split.max);
  }
}

void Driver::map() {
  logger_info("< map");
  Stopwatch sw;

  for (int i = 0; i < splitNums1; i++) {
    logger_debug("invoke dco.map, split: %d", (i + 1));

    DCO dco = lwdee::create_dco("MapDCO");
    // DCO dco = lwdee::create_dco(i + 2, "MapDCO");

    PartitionStep1 input(i, inputFile, sampleSplits);
    auto json = input.toJson();

    DDOId ddoId = dco.async("map", json);

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

  for (int i = 0; i < splitNums2; i++) {
    PartitionStep2 step2Input(i, outputFile);
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