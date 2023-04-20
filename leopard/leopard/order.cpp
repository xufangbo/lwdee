#include "order.hpp"

#include <unistd.h>
#include <iostream>
#include <istream>
#include <atomic>
#include "core/log.hpp"

std::atomic<int> acceptIndex = 0;

void registService() {
  TcpResponse::regist("com.cs.sales.order.save", [](BufferStream* inputStream, BufferStream* outputStream) {
    acceptIndex ++;

    // 1. inputStream -> order request parameter
    auto len = inputStream->get<uint32_t>();
    // auto content = inputStream->getString(len);

    std::istringstream s(inputStream->getString(len));
    int index;
    s >> index;

    printf("accept [%d / %d]\n", acceptIndex.load(),index);

    // 2. sale order business
    // sleep(1);

    // 3. outputStream
    std::string message = std::to_string(index) + " sales order persist succeed!";
    outputStream->put<uint32_t>(message.size());
    outputStream->put(message);
  });
}