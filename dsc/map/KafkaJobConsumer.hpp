#pragma once

#include <librdkafka/rdkafkacpp.h>
#include <thread>

class KafkaJobConsumer {
 private:
 static bool use_ccb;
  static std::thread thread;

  private:
  static void doStart(int partition);

 public:
  static void start(int partition);
};