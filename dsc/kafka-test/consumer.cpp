#include "consumer.hpp"

KafkaConsumer::KafkaConsumer(const std::string& brokers, const std::string& groupId, const std::vector<std::string>& topics, int partition) {
  m_brokers = brokers;
  m_groupId = groupId;
  m_topicVector = topics;
  m_partition = partition;

  // 创建Conf实例：
  m_config = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  if (m_config == nullptr) {
    std::cout << "Create Rdkafka Global Conf Failed." << std::endl;
  }

  m_topicConfig = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
  if (m_topicConfig == nullptr) {
    std::cout << "Create Rdkafka Topic Conf Failed." << std::endl;
  }

  // 设置Conf的各个配置参数：
  RdKafka::Conf::ConfResult result;
  std::string error_str;

  result = m_config->set("bootstrap.servers", m_brokers, error_str);
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Conf set 'bootstrap.servers' failed: " << error_str << std::endl;
  }

  result = m_config->set("group.id", m_groupId, error_str);  // 设置消费组名：group.id（string类型）
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Conf set 'group.id' failed: " << error_str << std::endl;
  }

  result = m_config->set("max.partition.fetch.bytes", "1024000", error_str);  // 消费消息的最大大小
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Conf set 'max.partition.fetch.bytes' failed: " << error_str << std::endl;
  }

  result = m_config->set("enable.partition.eof", "false", error_str);  // enable.partition.eof: 当消费者到达分区结尾，发送 RD_KAFKA_RESP_ERR__PARTITION_EOF 事件，默认值 true
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Conf set 'enable.partition.eof' failed: " << error_str << std::endl;
  }

  m_event_cb = new ConsumerEventCb;
  result = m_config->set("event_cb", m_event_cb, error_str);
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Conf set 'event_cb' failed: " << error_str << std::endl;
  }

  m_rebalance_cb = new ConsumerRebalanceCb;
  result = m_config->set("rebalance_cb", m_rebalance_cb, error_str);
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Conf set 'rebalance_cb' failed: " << error_str << std::endl;
  }

  // 设置 topic_conf的配置项：
  result = m_topicConfig->set("auto.offset.reset", "latest", error_str);
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Topic Conf set 'auto.offset.reset' failed: " << error_str << std::endl;
  }

  result = m_config->set("default_topic_conf", m_topicConfig, error_str);
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Conf set 'default_topic_conf' failed: " << error_str << std::endl;
  }

  // 创建消费者客户端：
  m_consumer = RdKafka::KafkaConsumer::create(m_config, error_str);
  if (m_consumer == nullptr) {
    std::cout << "Create KafkaConsumer failed: " << error_str << std::endl;
  }
  std::cout << "Create KafkaConsumer succeed, consumer name : " << m_consumer->name() << std::endl;
}

void KafkaConsumer::pullMessage() {
}

int main() {

  std::string brokers = ",10.180.98.132:9092,10.180.98.133:9092";
  std::string topic = "test";

  std::vector<std::string> topics;  // 待消费主题的集合
  topics.push_back(topic);

  std::string group = "consumer-group-demo";  // 消费组

  KafkaConsumer consumer(brokers, group, topics, RdKafka::Topic::OFFSET_BEGINNING);

  consumer.pullMessage();

  RdKafka::wait_destroyed(5000);

  return 0;
}

/*
在生产者/消费者 客户端 连接 broker 时，填充“bootstrap.server” 参数：
        Q: 为什么只设置了一个broker的地址（port = 9092），如果Kafka集群中有多个broker，且生产者/消费者订阅的Topic横跨多个broker时，生产者是如何知道其他broker的？
        A: bootstrap.server 参数用来指定生产者客户端连接Kafka集群所需的broker地址清单，具体的内容格式为：
        host1:port1, host2:port2
        可以设置一个或多个地址，中间以逗号隔开，此参数的默认值为 " "。
        【注意这里并非需要所有的broker地址，因为生产者会从给定的broker里查找到其他broker信息。】
        不过建议至少要设置两个以上的broker地址信息，当其中任意一个宕机时，生产者仍然可以连接到Kafka集群上。
*/
