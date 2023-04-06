#include "JobManager.hpp"

std::vector<KafkaJobConsumer*> JobManager::consumers;

void JobManager::add(KafkaJobConsumer* consumer) {
  consumers.push_back(consumer);
}