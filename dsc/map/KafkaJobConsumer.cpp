#include "KafkaJobConsumer.hpp"
#include "KafkaCallback.hpp"
#include "core/DscConfig.hpp"

static std::string errstr;

bool KafkaJobConsumer::use_ccb = true;
std::thread KafkaJobConsumer::thread;

void KafkaJobConsumer::start(int partition) {
  KafkaJobConsumer::thread = std::thread(&KafkaJobConsumer::doStart, partition);
}

void KafkaJobConsumer::doStart(int partition) {
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
  RdKafka::ErrorCode resp = consumer->start(topic, partition, RdKafka::Topic::OFFSET_STORED);
  if (resp != RdKafka::ERR_NO_ERROR) {
    std::cerr << "Failed to start consumer: " << RdKafka::err2str(resp) << std::endl;
    exit(1);
  }

  ExampleConsumeCb ex_consume_cb;

  /*
   * Consume messages
   */
  while (true) {
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