#include "order.hpp"

#include <unistd.h>
#include <iostream>
#include "core/log.hpp"

void registService() {
  TcpResponse::regist("com.cs.sales.order.save", [](BufferStream* inputStream, BufferStream* outputStream) {
    // 1. inputStream -> order request parameter
    auto len = inputStream->get<uint32_t>();
    auto content = inputStream->getString(len);

    // logger_info("accept (%d) :  %s", len, content.c_str());

    // 2. sale order business
    // sleep(1);

    // 3. outputStream
    std::string message = "sales order persist succeed!";
    outputStream->put<uint32_t>(message.size());
    outputStream->put(message);
  });
}