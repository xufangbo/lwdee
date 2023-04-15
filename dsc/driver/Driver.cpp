#include "Driver.h"

#include <algorithm>
#include <fstream>
#include <vector>

#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "matrix/LinuxMatrix.h"
#include "client/TcpRequest.hpp"
#include "client/SocketScheduler.hpp"

void Driver::startJob() {
  try {
    logger_info("> job");
    // LinuxMatrix::print();

    this->conf = DscConfig::instance();

    if (conf->partitions.size() > conf->splitNums1) {
      throw Exception("kafka partiton cout can't be more than map partitons cout", ZONE);
    }

    Stopwatch sw;

    this->start_reduce();

    this->start_map();

    this->start_kafka();
    // this->mapToReduce();

    // LinuxMatrix::print();
    logger_info("< job,eclipse %lf s", sw.stop());

    logger_info("finished");
  } catch (Exception& ex) {
    logger_error("execute driver error,%s", ex.getMessage().c_str());
  } catch (std::exception& ex) {
    logger_error("execute driver error,%s", ex.what());
  }
}

void Driver::start_kafka() {
  logger_info("< start kafka");
  Stopwatch sw;

  for (int i = 0; i < conf->partitions.size(); i++) {
    printf("\n");
    logger_info("start kafka %d / %d -----------------", i, conf->partitions.size());

    std::vector<string> kafka_mapVoxors;
    for (int m = i; m < conf->splitNums1; m = m + conf->partitions.size()) {
      kafka_mapVoxors.push_back(mapVoxorIds[m]);
    }
    PartitionKafka input(i, conf->group, conf->topic, conf->window, kafka_mapVoxors);

    auto client = SocketScheduler::newClient(conf->ip.c_str(), conf->port);
    RequestCallback callback = [](BufferStream* inputStream) {
      auto len = inputStream->get<uint32_t>();
      auto content = inputStream->getString(len);

      logger_info("recive(%d) :  %s", len, content.c_str());
    };

  auto json = input.toJson();
  client->invoke(ServicePaths::kafka_start, (void*)json.c_str(), json.size(), callback);
  client->wait();

  logger_info("> start kafka,eclipse %lf", sw.stop());
}
}

void Driver::start_map() {
  logger_debug("< map");
  Stopwatch sw;

  for (int i = 0; i < conf->splitNums1; i++) {
    printf("\n");
    logger_debug("start map %d / %d -----------------", i, conf->splitNums1);

    DCO dco = lwdee::create_dco("MapDCO");
    mapVoxorIds.push_back(dco.voxorId());
    logger_debug("map voxorId: %s", dco.voxorId().c_str());

    PartitionMap input(i, reduceVoxorIds);
    DDOId ddoId = dco.async("start", input.toJson());
    logger_debug("%s", input.toJson().c_str());

    mapInvokers.push_back(std::make_pair(dco, ddoId));
  }
  
 auto client = SocketScheduler::newClient(conf->ip.c_str(), conf->port);
    RequestCallback callback = [](BufferStream* inputStream) {
      auto len = inputStream->get<uint32_t>();
      auto content = inputStream->getString(len);

      logger_info("recive(%d) :  %s", len, content.c_str());
    };

  auto json = input.toJson();
  client->invoke(ServicePaths::map_start, (void*)json.c_str(), json.size(), callback);
  client->wait();  

  logger_debug("> start map,eclipse %lf", sw.stop());
}

void Driver::start_reduce() {
  logger_warn("< start reduce");
  Stopwatch sw;

  for (int i = 0; i < conf->splitNums2; i++) {
    printf("\n");
    logger_warn("start reduce %d / %d -----------------", i, conf->splitNums2);

    DCO dco = lwdee::create_dco("ReduceDCO");
    reduceVoxorIds.push_back(dco.voxorId());
    logger_warn("reduce voxorId: %s", dco.voxorId().c_str());

    PartitionReduce input(i);
    DDOId ddoId = dco.async("start", input.toJson());
    // logger_warn("%s", input.toJson().c_str());

    reduceInvokers.push_back(std::make_pair(dco, ddoId));
  }
  
 auto client = SocketScheduler::newClient(conf->ip.c_str(), conf->port);
    RequestCallback callback = [](BufferStream* inputStream) {
      auto len = inputStream->get<uint32_t>();
      auto content = inputStream->getString(len);

      logger_info("recive(%d) :  %s", len, content.c_str());
    };

  auto json = input.toJson();
  client->invoke(ServicePaths::reduce_start, (void*)json.c_str(), json.size(), callback);
  client->wait();  

  logger_warn("> start reduce,eclipse %lf", sw.stop());
}