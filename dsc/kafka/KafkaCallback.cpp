#include "KafkaCallback.hpp"
#include <iostream>

static bool exit_eof = false;

void msg_consume(RdKafka::Message* message, void* opaque) {
  const RdKafka::Headers* headers;

  switch (message->err()) {
    case RdKafka::ERR__TIMED_OUT:
      break;

    case RdKafka::ERR_NO_ERROR:
      /* Real message */
      std::cout << "Read msg at offset " << message->offset() << ": ";
      if (message->key()) {
        std::cout << "Key: " << *message->key() << std::endl;
      }
      headers = message->headers();
      if (headers) {
        std::vector<RdKafka::Headers::Header> hdrs = headers->get_all();
        for (size_t i = 0; i < hdrs.size(); i++) {
          const RdKafka::Headers::Header hdr = hdrs[i];

          if (hdr.value() != NULL)
            printf(" Header: %s = \"%.*s\"\n", hdr.key().c_str(), (int)hdr.value_size(), (const char*)hdr.value());
          else
            printf(" Header:  %s = NULL\n", hdr.key().c_str());
        }
      }
      printf("%.*s\n", static_cast<int>(message->len()), static_cast<const char*>(message->payload()));
      break;

    case RdKafka::ERR__PARTITION_EOF:
      // logger_warn("kafka partition eof");
      break;

    case RdKafka::ERR__UNKNOWN_TOPIC:
    case RdKafka::ERR__UNKNOWN_PARTITION:
      logger_error("Consume failed: %s",message->errstr());
      break;

    default:
     logger_error("Consume failed: %s",message->errstr());
  }
}

void showMetadata(RdKafka::Consumer* consumer, RdKafka::Topic* topic) {
  class RdKafka::Metadata* metadata;

  /* Fetch metadata */
  RdKafka::ErrorCode err = consumer->metadata(false, topic, &metadata, 5000);
  if (err != RdKafka::ERR_NO_ERROR) {
    std::cerr << "%% Failed to acquire metadata: " << RdKafka::err2str(err) << std::endl;
    return;
  }

  metadata_print(topic->name(), metadata);

  delete metadata;
}

void metadata_print(const std::string& topic, const RdKafka::Metadata* metadata) {
  std::cout << "Metadata for " << (topic.empty() ? "" : "all topics") << "(from broker " << metadata->orig_broker_id() << ":" << metadata->orig_broker_name() << std::endl;

  /* Iterate brokers */
  std::cout << " " << metadata->brokers()->size() << " brokers:" << std::endl;
  RdKafka::Metadata::BrokerMetadataIterator ib;
  for (ib = metadata->brokers()->begin(); ib != metadata->brokers()->end(); ++ib) {
    std::cout << "  broker " << (*ib)->id() << " at " << (*ib)->host() << ":" << (*ib)->port() << std::endl;
  }
  /* Iterate topics */
  std::cout << metadata->topics()->size() << " topics:" << std::endl;
  RdKafka::Metadata::TopicMetadataIterator t;
  for (t = metadata->topics()->begin(); t != metadata->topics()->end(); ++t) {
    std::cout << "  topic \"" << (*t)->topic() << "\" with " << (*t)->partitions()->size() << " partitions:";

    if ((*t)->err() != RdKafka::ERR_NO_ERROR) {
      std::cout << " " << err2str((*t)->err());
      if ((*t)->err() == RdKafka::ERR_LEADER_NOT_AVAILABLE)
        std::cout << " (try again)";
    }
    std::cout << std::endl;

    /* Iterate topic's partitions */
    RdKafka::TopicMetadata::PartitionMetadataIterator p;
    for (p = (*t)->partitions()->begin(); p != (*t)->partitions()->end(); ++p) {
      std::cout << "    partition " << (*p)->id() << ", leader " << (*p)->leader() << ", replicas: ";

      /* Iterate partition's replicas */
      RdKafka::PartitionMetadata::ReplicasIterator r;
      for (r = (*p)->replicas()->begin(); r != (*p)->replicas()->end(); ++r) {
        std::cout << (r == (*p)->replicas()->begin() ? "" : ",") << *r;
      }

      /* Iterate partition's ISRs */
      std::cout << ", isrs: ";
      RdKafka::PartitionMetadata::ISRSIterator iis;
      for (iis = (*p)->isrs()->begin(); iis != (*p)->isrs()->end(); ++iis)
        std::cout << (iis == (*p)->isrs()->begin() ? "" : ",") << *iis;

      if ((*p)->err() != RdKafka::ERR_NO_ERROR)
        std::cout << ", " << RdKafka::err2str((*p)->err()) << std::endl;
      else
        std::cout << std::endl;
    }
  }
}