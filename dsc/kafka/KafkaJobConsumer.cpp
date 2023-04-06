#include "KafkaJobConsumer.hpp"
#include "KafkaCallback.hpp"
#include "core/DscConfig.hpp"

static std::string errstr;

void KafkaJobConsumer::start(std::shared_ptr<PartitionKafka> input) {
  this->input = input;

  for (int i = 0; i < input->mapCount; i++) {
    DCO dco = lwdee::create_dco("MapDCO");
    mapDocs.push_back(dco);
  }
  KafkaJobConsumer::thread = std::thread(&KafkaJobConsumer::doStart, this);
}

void KafkaJobConsumer::doStart() {
  auto dsconf = DscConfig::instance();
  /*
   * Create configuration objects
   */
  RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

  // conf->set("metadata.broker.list", brokers, errstr);
  conf->set("bootstrap.servers", dsconf->brokers, errstr);
  conf->set("group.id", dsconf->group, errstr);  // 设置消费组名：group.id（string类型）

  conf->set("max.partition.fetch.bytes", "1024000", errstr);  // 消费消息的最大大小
  conf->set("enable.partition.eof", "true", errstr);          // 当消费者到达分区结尾，发送 RD_KAFKA_RESP_ERR__PARTITION_EOF 事件，默认值 true

  ExampleEventCb ex_event_cb;
  conf->set("event_cb", &ex_event_cb, errstr);

  /*
   * Create consumer using accumulated global configuration.
   */
  RdKafka::Consumer* consumer = RdKafka::Consumer::create(conf, errstr);
  if (!consumer) {
    std::cerr << "Failed to create consumer: " << errstr << std::endl;
    exit(1);
  }

  /*
   * Create topic handle.
   */
  RdKafka::Conf* tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

  RdKafka::Topic* topic = RdKafka::Topic::create(consumer, dsconf->topic, tconf, errstr);
  if (!topic) {
    std::cerr << "Failed to create topic: " << errstr << std::endl;
    exit(1);
  }

  // showMetadata(consumer, topic);

  /*
   * Start consumer for topic+partition at start offset
   */
  RdKafka::ErrorCode resp = consumer->start(topic, input->index, RdKafka::Topic::OFFSET_STORED);
  if (resp != RdKafka::ERR_NO_ERROR) {
    std::cerr << "Failed to start consumer: " << RdKafka::err2str(resp) << std::endl;
    exit(1);
  }

  /*
   * Consume messages
   */
  while (true) {
    RdKafka::Message* msg = consumer->consume(topic, input->index, 1000);
    this->msg_consume(msg, NULL);
    delete msg;
    consumer->poll(0);
  }

  /*
   * Stop consumer
   */
  consumer->stop(topic, input->index);

  consumer->poll(1000);

  delete topic;
  delete consumer;
}

void KafkaJobConsumer::msg_consume(RdKafka::Message* message, void* opaque) {
  switch (message->err()) {
    case RdKafka::ERR__TIMED_OUT:
      break;

    case RdKafka::ERR_NO_ERROR:
      std::cout << "Read msg at offset " << message->offset() << ": ";
      printf("%.*s\n", static_cast<int>(message->len()), static_cast<const char*>(message->payload()));
      logger_debug("map size %d", input->mapCount);
      break;

    case RdKafka::ERR__PARTITION_EOF:
      // logger_warn("kafka partition eof");
      break;

    case RdKafka::ERR__UNKNOWN_TOPIC:
    case RdKafka::ERR__UNKNOWN_PARTITION:
      logger_error("Consume failed: %s", message->errstr());
      break;

    default:
      logger_error("Consume failed: %s", message->errstr());
  }
}