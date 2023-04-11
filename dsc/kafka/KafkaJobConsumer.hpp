#pragma once

#include <librdkafka/rdkafkacpp.h>
#include <thread>
#include "core/Partition.h"
#include "ToMap.h"


class KafkaJobConsumer {
 private:
  PartitionKafka *input;
  std::thread thread;
  ToMap toMap;

 private:
  void doStart();
  void msg_consume(RdKafka::Message* message, void* opaque);

 public:
  void start(PartitionKafka *input);
};