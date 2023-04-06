#pragma once

#include <librdkafka/rdkafkacpp.h>
#include <thread>

class KafkaJobConsumer {
 private:
  bool use_ccb = true;
  std::thread thread;

 private:
  void doStart(int partition);

 public:
  void start(int partition);
};