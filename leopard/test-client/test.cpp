#include "test.hpp"

#include <atomic>
#include <fstream>
#include <sstream>
#include "src/Leopard.hpp"
#include "src/LaneClient.hpp"
#include "src/LaneClients.hpp"
#include "core/Exception.hpp"
#include "core/log.hpp"
#include "core/suspend.hpp"
#include "src/Epoll.hpp"
#include "src/LeopardConfig.hpp"
#include "src/Socket.hpp"

std::atomic<int> responseIndex = 0;
std::string path = "com.cs.sales.order.save";

RequestCallback callback = [](BufferStream* inputStream) {
  responseIndex++;
  auto len = inputStream->get<uint32_t>();
  std::istringstream s(inputStream->getString(len));
  int index;
  s >> index;
  // logger_trace("recive [%d]", index);
  // logger_trace("recive [callback:%d/content:%d]", responseIndex.load(), index);
};

void TestSync::execute(TestReport& testReport, int testSize, TestInput& inputType, int parallel, std::string ip, int port, float timeout) {
  Stopwatch sw;
  for (int i = 0; i < testSize; i++) {
    try {
      auto client = LaneClient::create(ip, port);

      auto input = std::to_string(i + 1) + " " + inputType();
      SocketWaiter waiter = client->invoke(path, (void*)input.c_str(), input.size(), callback);

      auto time = waiter->wait(timeout);
      client->close();
      logger_trace("wait finished %d eclipse %.3lfs ", i + 1, time);

    } catch (Exception& ex) {
      logger_warn("%s", ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("%s,%s:%d", ex.what(), __FILE__, __LINE__);
    }
  }

  testReport.writeLine(name, inputType.name, testSize, 1, sw.elapsed());
  auto eclapsed = sw.stop();
  logger_debug("%s,input:%s,testSize:%d,parallel:%d,elapsed %.3f,ave eclapsed:%.3f,response count : %d,", name.c_str(), inputType.name.c_str(), testSize, 1, eclapsed, eclapsed / testSize, responseIndex.load());
}
void TestAsync::execute(TestReport& testReport, int testSize, TestInput& inputType, int parallel, std::string ip, int port, float timeout) {
  Stopwatch sw;

  LaneClients clients;
  for (int i = 0; i < testSize; i++) {
    try {
      auto client = clients.create(ip, port);

      auto input = std::to_string(i + 1) + " " + inputType();
      client->invoke(path, (void*)input.c_str(), input.size(), callback);

    } catch (Exception& ex) {
      logger_warn("%s", ex.getMessage().c_str(),ex.getStackTrace().c_str());
    } catch (std::exception& ex) {
      logger_error("%s,%s:%d", ex.what(), __FILE__, __LINE__);
    }
  }

  clients.wait(timeout);
  clients.close();

  testReport.writeLine(name, inputType.name, testSize, testSize, sw.elapsed());
  auto eclapsed = sw.stop();
  logger_debug("%s,input:%s,testSize:%d,parallel:%d,elapsed %.3f,ave eclapsed:%.3f,response count : %d,", name.c_str(), inputType.name.c_str(), testSize, testSize, eclapsed, eclapsed / testSize, responseIndex.load());
}

void TestLongConnect::execute(TestReport& testReport, int testSize, TestInput& inputType, int parallel, std::string ip, int port, float timeout) {
  auto client = LaneClient::create(ip, port, parallel);

  Stopwatch sw;
  for (int i = 0; i < testSize; i++) {
    try {
      auto input = std::to_string(i + 1) + " " + inputType();
      client->invoke(path, (void*)input.c_str(), input.size(), callback);

    } catch (Exception& ex) {
      logger_warn("%s,\n%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
    } catch (std::exception& ex) {
      logger_error("%s,%s:%d", ex.what(), __FILE__, __LINE__);
    }
  }
  client->wait(timeout);
  client->close();

  testReport.writeLine(this->name, inputType.name, testSize, parallel, sw.elapsed());

  auto eclapsed = sw.stop();
  logger_debug("%s,input:%s,testSize:%d,parallel:%d,elapsed %.3f,ave eclapsed:%.3f,response count : %d,", name.c_str(), inputType.name.c_str(), testSize, parallel, eclapsed, eclapsed / testSize, responseIndex.load());
}

void TestReport::writeTitle() {
  std::ofstream f(fileName, std::ios_base::trunc);
  if (!f.is_open()) {
    logger_error("can't open file %s", fileName.c_str());
  }

  f << "no" << split;  
  f << "连接类型" << split;
  f << "请求数据" << split;
  f << "请求次数" << split;
  f << "Socket并发" << split;
  f << "耗时" << split;
  f << "平均耗时" << split;

  f << std::endl;

  f.flush();
  f.close();
}

void TestReport::writeEmptyLine() {
  std::ofstream f(fileName, std::ios_base::app);
  if (!f.is_open()) {
    logger_error("can't open file %s", fileName.c_str());
  }
  f << split << std::endl;

  f.flush();
  f.close();
}

void TestReport::writeLine(std::string type, std::string inputType, int testSize, int parallel, float elapsed) {
  seq++;

  std::ofstream f(fileName, std::ios_base::app);
  if (!f.is_open()) {
    logger_error("can't open file %s", fileName.c_str());
  }

  f << seq << split;
  f << type << split;
  f << inputType << split;  
  f << testSize << split;
  f << parallel << split;
  f << std::fixed << std::setprecision(3) << elapsed << split;
  f << std::fixed << std::setprecision(3) << elapsed / testSize << split;

  f << std::endl;

  f.flush();
  f.close();
}