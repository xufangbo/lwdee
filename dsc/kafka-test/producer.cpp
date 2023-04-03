#include <librdkafka/rdkafkacpp.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

std::string brokers = "10.180.98.131:9092,10.180.98.132:9092,10.180.98.133:9092";
std::string topic = "test";

void sendMessage(RdKafka::Producer* producer);

int main() {
  std::cout << "hello" << std::endl;

  RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

  std::string errstr;
  if (conf->set("bootstrap.servers", brokers, errstr) != RdKafka::Conf::CONF_OK) {
    std::cerr << errstr << std::endl;
    exit(1);
  }

  RdKafka::Producer* producer = RdKafka::Producer::create(conf, errstr);
  if (!producer) {
    std::cerr << "Failed to create producer: " << errstr << std::endl;
    exit(1);
  }

  for (;;) {
    sendMessage(producer);
    sleep(1);
  }

  return 1;
}

void sendMessage(RdKafka::Producer* producer) {
  time_t timep;
  time(&timep);
  char tmp[64];
  strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));

  std::string line = std::string("pulse data ") + tmp;

  RdKafka::ErrorCode err = producer->produce(
      topic,                                             /* Topic name */
      RdKafka::Topic::PARTITION_UA,                      /* Any Partition: the builtin partitioner will be used to assign the message to a topic based on the message key, or random partition if the key is not set. */
      RdKafka::Producer::RK_MSG_COPY /* Copy payload */, /* Make a copy of the value */
      const_cast<char*>(line.c_str()), line.size(),      /* Value */
      NULL, 0,                                           /* Key */
      0,                                                 /* Timestamp (defaults to current time) */
      NULL,                                              /* Message headers, if any */
      NULL);                                             /* Per-message opaque value passed to  delivery report */

  if (err != RdKafka::ERR_NO_ERROR) {
    std::cerr << "% Failed to produce to topic " << topic << ": " << RdKafka::err2str(err) << std::endl;

    if (err == RdKafka::ERR__QUEUE_FULL) {
      /* If the internal queue is full, wait for messages to be delivered and then retry.
       * The internal queue represents both messages to be sent and messages that have been sent or failed, awaiting their
       * delivery report callback to be called.
       * The internal queue is limited by the configuration property queue.buffering.max.messages */
      producer->poll(1000 /*block for max 1000ms*/);
    }
  } else {
    std::cerr << "- send message : " << line << " [" << topic << "]"
              << " [" << line.size() << " bytes]" << std::endl;
  }
}