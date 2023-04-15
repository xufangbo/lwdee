#include "KafkaDCO.h"

#include <iostream>
#include <sstream>

#include "JobManager.hpp"
#include "KafkaJobConsumer.hpp"
#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "matrix/LinuxMatrix.h"
#include "net/BufferStream.hpp"
#include "server/TcpResponse.hpp"

std::string KafkaDCO::start(std::string a) {
  printf("\n");
  try {
    LinuxMatrix::start();

    logger_trace("kafka dco accept ");
    Stopwatch sw;

    input.fromJson(&a);

    KafkaJobConsumer* consumer = new KafkaJobConsumer();
    JobManager::add(consumer);
    consumer->start(&input);

    LinuxMatrix::stream.kafka_index = input.index;
    // LinuxMatrix::print();
    logger_trace("> accept start ,partition : %d,eclipse %lf", input.index,
                 sw.stop());

    return "succeed";

  } catch (Exception& ex) {
    logger_error("step2 failed,%s,%s", ex.getMessage().c_str(),
                 ex.getStackTrace().c_str());
    return "failed";
  } catch (std::exception& ex) {
    logger_error("step2 failed,%s", ex.what());
    return "failed";
  }
}

void regist_kafka_start_service() {
  TcpResponse::regist(
      ServicePaths::kafka_start,
      [](BufferStream* inputStream, BufferStream* outputStream) {
        // 1. inputStream -> order request parameter
        auto len = inputStream->get<uint32_t>();
        auto content = inputStream->getString(len);

        KafkaDCO::start(content);

        // 3. outputStream
        std::string message = "succeed";
        outputStream->put<uint32_t>(message.size());
        outputStream->put(message);
      });
}