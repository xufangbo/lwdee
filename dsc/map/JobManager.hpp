#pragma once

#include <iostream>
#include <vector>
#include "KafkaJobConsumer.hpp"

class JobManager {
 private:
  static std::vector<KafkaJobConsumer*> consumers;

 public:
  static void add(KafkaJobConsumer* consumer);
};