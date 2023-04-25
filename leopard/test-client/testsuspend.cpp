#include "testsuspend.hpp"

#ifdef LEOPARD_SUSPEND
suspend testSuspend(LaneClient* client, int i) {
  std::string input = "green green green " + std::to_string(i);
  auto path = "com.cs.sales.order.save";

  logger_debug("send %s", input.c_str());
  BufferStream* inputStream = co_await client->invoke(path, (void*)input.c_str(), input.size());

  auto len = inputStream->get<uint32_t>();
  auto content = inputStream->getString(len);

  logger_info("recive(%d) :  %s", len, content.c_str());
}
#endif

// client->invoke(
//     "com.cs.sales.order.save",
//     [](BufferStream* outputStream) {
//       std::string input = "white1 white2 white3 !";
//       outputStream->put<uint32_t>(input.size());
//       outputStream->put(input);
//       logger_warn("send %s", input.c_str());
//     },
//     [](BufferStream* inputStream) {
//       auto len = inputStream->get<uint32_t>();
//       auto content = inputStream->getString(len);

//       logger_info("recive(%d) :  %s", len, content.c_str());
//     });
