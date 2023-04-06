#pragma once

#include <librdkafka/rdkafkacpp.h>
#include "core/log.hpp"

static void metadata_print(const std::string& topic, const RdKafka::Metadata* metadata);
void showMetadata(RdKafka::Consumer* consumer, RdKafka::Topic* topic);
void msg_consume(RdKafka::Message* message, void* opaque);

class ExampleConsumeCb : public RdKafka::ConsumeCb {
 public:
  void consume_cb(RdKafka::Message& msg, void* opaque) {
    msg_consume(&msg, opaque);
  }
};

class ExampleEventCb : public RdKafka::EventCb {
 public:
  void event_cb(RdKafka::Event& event) {
    switch (event.type()) {
      case RdKafka::Event::EVENT_ERROR:
        if (event.fatal()) {
          logger_error("consumer FATAL");
        }
        logger_error("consumer ERROR(%s) : %s", RdKafka::err2str(event.err()).c_str(), event.str().c_str());
        break;

      case RdKafka::Event::EVENT_STATS:
        logger_warn("consumer status : %s", event.str().c_str());
        break;

      case RdKafka::Event::EVENT_LOG:
        logger_debug("LOG-%i-%s: %s", event.severity(), event.fac().c_str(), event.str().c_str());
        break;

      default:
        logger_debug("consumer EVENT %d(%s) : %s", event.type(), RdKafka::err2str(event.err()).c_str(), event.str().c_str());
        break;
    }
  }
};