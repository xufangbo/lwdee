#include "Antelope.hpp"
#include "LaneClient.hpp"
#include "core/Exception.hpp"
#include "core/log.hpp"
#include "core/suspend.hpp"
#include "net/Epoll.hpp"
#include "net/LeopardConfig.hpp"
#include "net/Socket.hpp"
#include "test.hpp"
#include "testsuspend.hpp"

int main(int argc, char** argv) {
  for (int i = 0; i < 100; i++) {
    printf("\n");
  }
  read_log_config("client");

  auto* conf = LeopardConfig::instance();
  conf->readConfig();
  auto ip = conf->ip;
  auto port = conf->port;
  logger_trace("%s:%d", conf->ip.c_str(), conf->port);

  Antelope::instance.start();

  // test_short_sync(1, input_small, ip, port);         // ( 1682005236163 -> 10 -> 10 -> 12 ) <==> ( 0.010 + 0.000 + 0.002 = 0.012 )
  test_short_sync(1000, input_small, ip, port);  // 10.282 leopard: 28.304  ( 1682094116562 -> 4294967267 -> 4294967268 -> 22 ) <==> ( 4294967.500 + 0.001 + 0.050 = 0.022 )
  // test_short_async(1000, input_small, ip, port);  // 0.259 , leopard: 10.03s
  // test_long_sync(1, input_small, 1, ip, port);       // 0.020
  // test_long_sync(1000, input_small, 1, ip, port);    // 2.198  leopard: 0.429
  // test_long_sync(1000, input_small, 10, ip, port);   // 0.158  leopard: 
  // test_long_sync(1000, input_small, 100, ip, port);  // blocked always , add lock not ok

  //  test_short_sync(1, input_large, ip, port);  // 0.082
  // test_short_sync(1000, input_large, ip, port);  // 72.791  leopard: 235.771  ( 1682093907194 -> 110 -> 111 -> 151 ) <==> ( 0.110 + 0.001 + 0.040 = 0.151 )
  // test_short_async(1000, input_large, ip, port);     //
  // test_long_sync(1, input_large, 1, ip, port);       //
  // test_long_sync(1000, input_large, 1, ip, port);    //
  // test_long_sync(1000, input_large, 10, ip, port);   //
  // test_long_sync(1000, input_large, 100, ip, port);  //

  // for (int i = 0; i < 100; i++) {
  //   test_short_sync(1000, input_small, ip, port);  // 10.282
  //   // test_short_async(1000, input_small, ip, port);  // 0.259 , offen  server or client segement failed
  //   // test_long_sync(1000, input_small, 10, ip, port);  // 0.020

  //   for (int t = 0; t < 30; t++) {
  //     printf("\n");
  //   }
  //   printf("%d ^^^^^^^^^^^^^^^^", i);
  //   sleep(1);
  // }

  Antelope::instance.join();
  // Antelope::instance.stop();

  return 0;
}
