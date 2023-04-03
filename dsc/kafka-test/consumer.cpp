#include <librdkafka/rdkafkacpp.h>
#include <time.h>
#include <unistd.h>
#include <csignal>
#include <iostream>

std::string brokers = "10.180.98.131:9092,10.180.98.132:9092,10.180.98.133:9092";
std::string topicName = "test";
int32_t partition = 0;  // RdKafka::Topic::PARTITION_UA;
std::string errstr;

static volatile sig_atomic_t run = 1;
static bool exit_eof = false;
int use_ccb = true;

int64_t start_offset = RdKafka::Topic::OFFSET_BEGINNING;

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
          std::cerr << "FATAL ";
          run = 0;
        }
        std::cerr << "ERROR (" << RdKafka::err2str(event.err()) << "): " << event.str() << std::endl;
        break;

      case RdKafka::Event::EVENT_STATS:
        std::cerr << "\"STATS\": " << event.str() << std::endl;
        break;

      case RdKafka::Event::EVENT_LOG:
        fprintf(stderr, "LOG-%i-%s: %s\n", event.severity(), event.fac().c_str(), event.str().c_str());
        break;

      default:
        std::cerr << "EVENT " << event.type() << " (" << RdKafka::err2str(event.err()) << "): " << event.str() << std::endl;
        break;
    }
  }
};

int main() {
  /*
   * Create configuration objects
   */
  RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  RdKafka::Conf* tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

   conf->set("metadata.broker.list", brokers, errstr);

  ExampleEventCb ex_event_cb;
  conf->set("event_cb", &ex_event_cb, errstr);

  /*
   * Consumer mode
   */

  conf->set("enable.partition.eof", "true", errstr);

  /*
   * Create consumer using accumulated global configuration.
   */
  RdKafka::Consumer* consumer = RdKafka::Consumer::create(conf, errstr);
  if (!consumer) {
    std::cerr << "Failed to create consumer: " << errstr << std::endl;
    exit(1);
  }

  std::cout << "% Created consumer " << consumer->name() << std::endl;

  /*
   * Create topic handle.
   */
  RdKafka::Topic* topic = RdKafka::Topic::create(consumer, topicName, tconf, errstr);
  if (!topic) {
    std::cerr << "Failed to create topic: " << errstr << std::endl;
    exit(1);
  }

  /*
   * Start consumer for topic+partition at start offset
   */
  RdKafka::ErrorCode resp = consumer->start(topic, partition, start_offset);
  if (resp != RdKafka::ERR_NO_ERROR) {
    std::cerr << "Failed to start consumer: " << RdKafka::err2str(resp)
              << std::endl;
    exit(1);
  }

  ExampleConsumeCb ex_consume_cb;

  /*
   * Consume messages
   */
  while (run) {
    if (use_ccb) {
      consumer->consume_callback(topic, partition, 1000, &ex_consume_cb, &use_ccb);
    } else {
      RdKafka::Message* msg = consumer->consume(topic, partition, 1000);
      msg_consume(msg, NULL);
      delete msg;
    }
    consumer->poll(0);
  }

  /*
   * Stop consumer
   */
  consumer->stop(topic, partition);

  consumer->poll(1000);

  delete topic;
  delete consumer;
}

void msg_consume(RdKafka::Message* message, void* opaque) {
  const RdKafka::Headers* headers;

  switch (message->err()) {
    case RdKafka::ERR__TIMED_OUT:
      break;

    case RdKafka::ERR_NO_ERROR:
      /* Real message */
      std::cout << "Read msg at offset " << message->offset() << std::endl;
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
      /* Last message */
      if (exit_eof) {
        run = 0;
      }
      break;

    case RdKafka::ERR__UNKNOWN_TOPIC:
    case RdKafka::ERR__UNKNOWN_PARTITION:
      std::cerr << "Consume failed: " << message->errstr() << std::endl;
      run = 0;
      break;

    default:
      /* Errors */
      std::cerr << "Consume failed: " << message->errstr() << std::endl;
      run = 0;
  }
}