#include <librdkafka/rdkafkacpp.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

#include "core/DscConfig.hpp"
#include "core/log.hpp"

#define BUFFER_SIZE 1024

void sendMessage(RdKafka::Producer* producer, char* line);
void init_logger(std::string nodeName);

DscConfig* dsconf = DscConfig::instance();

int main() {
  init_logger("producer");

  dsconf->readConfig();

  RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

  std::string errstr;
  if (conf->set("bootstrap.servers", dsconf->brokers, errstr) != RdKafka::Conf::CONF_OK) {
    std::cerr << errstr << std::endl;
    exit(1);
  }

  RdKafka::Producer* producer = RdKafka::Producer::create(conf, errstr);
  if (!producer) {
    std::cerr << "Failed to create producer: " << errstr << std::endl;
    exit(1);
  }

  while (true) {
    FILE* fd = fopen(dsconf->inputFile.c_str(), "r");
    if (fd == NULL) {
      logger_error("can't open file %s", dsconf->inputFile.c_str());
      exit(1);
    }

    char buffer[BUFFER_SIZE];
    char* line = NULL;

    while (!feof(fd) && !ferror(fd)) {
      if (line = fgets(buffer, BUFFER_SIZE, fd)) {
        line[strlen(line) - 1] = '\0';
        line[strlen(line) - 1] = '\0';
        sendMessage(producer, line);
      }
      usleep(1000000 / 10);
    }

    fclose(fd);

    logger_info("eof");
  }

  return 1;
}

void sendMessage(RdKafka::Producer* producer, char* line) {
  RdKafka::ErrorCode err = producer->produce(
      dsconf->topic,                                     /* Topic name */
      RdKafka::Topic::PARTITION_UA,                      /* Any Partition: the builtin partitioner will be used to assign the message to a topic based on the message key, or random partition if the key is not set. */
      RdKafka::Producer::RK_MSG_COPY /* Copy payload */, /* Make a copy of the value */
      line, strlen(line),                                /* Value */
      NULL, 0,                                           /* Key */
      0,                                                 /* Timestamp (defaults to current time) */
      NULL,                                              /* Message headers, if any */
      NULL);                                             /* Per-message opaque value passed to  delivery report */

  if (err != RdKafka::ERR_NO_ERROR) {
    std::cerr << "% Failed to produce to topic " << dsconf->topic << ": " << RdKafka::err2str(err) << std::endl;

    if (err == RdKafka::ERR__QUEUE_FULL) {
      /* If the internal queue is full, wait for messages to be delivered and then retry.
       * The internal queue represents both messages to be sent and messages that have been sent or failed, awaiting their
       * delivery report callback to be called.
       * The internal queue is limited by the configuration property queue.buffering.max.messages */
      producer->poll(1000 /*block for max 1000ms*/);
    }
  } else {
    logger_trace("%s [%s] [%d bytes]", line, dsconf->topic.c_str(), strlen(line));
  }
}

void init_logger(std::string nodeName) {
  LogOption option;
  option.initalized = false;
  option.level = log_trace;
  strcpy(option.path, "./log");
  strcpy(option.name, nodeName.c_str());
  option.is_color = true;
  option.days = 10;

  if (logger_initialize(option) != 0) {
    printf("log initialize error\n");
  } else {
    logger_info("-- app starting ... ");
  }
}