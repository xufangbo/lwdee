#include "KafkaDCO.h"

#include <iostream>
#include <sstream>

#include "JobManager.hpp"
#include "KafkaJobConsumer.hpp"
#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/Stopwatch.h"
#include "core/UhconnVoxorFactory.h"
#include "core/UhconnWorkNode.h"
#include "core/log.hpp"
#include "matrix/LinuxMatrix.h"

std::vector<DDO> KafkaDCO::ddos;

std::string KafkaDCO::start(std::string a) {
  try {
    logger_trace("kafka dco accept ");
    Stopwatch sw;

    std::shared_ptr<PartitionKafka> input = std::make_shared<PartitionKafka>();
    input->fromJson(&a);

    KafkaJobConsumer* consumer = new KafkaJobConsumer();
    JobManager::add(consumer);
    consumer->start(input);

    LinuxMatrix::print();
    logger_trace("> accept start ,partition : %d,mapCount: %d,eclipse %lf", input->index, input->mapCount, sw.stop());

    return "succeed!";

  } catch (Exception& ex) {
    logger_error("step2 failed,%s,%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
    return "failed";
  } catch (std::exception& ex) {
    logger_error("step2 failed,%s", ex.what());
    return "failed";
  }
}

KafkaDCO::KafkaDCO() {
  getFunctionTable()["start"] = (PTR)&KafkaDCO::start;
  name = "KafkaDCO";
}
KafkaDCO::~KafkaDCO() {}
