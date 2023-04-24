#include "test_input.hpp"

TestReport testReport;

TestInput input_100_byte("100-bytes", 100);
TestInput input_1_K("1K", 1000);
TestInput input_10_K("10K", 10000);
TestInput input_100_K("100K", 100000);
TestInput input_1_M("1M", 1000000);
TestInput input_10_M("10M", 10000000);

TestSync test_sync;
TestAsync test_async;
TestLongConnect test_long;

void test_input(std::string ip, int port) {
  testReport.writeTitle();
  test_by_testor(&test_sync, 20, 1, ip, port);
  test_by_testor(&test_async, 20, 20, ip, port);
  test_by_testor(&test_long, 20, 1, ip, port);
  test_by_testor(&test_long, 20, 10, ip, port);

  // testReport.writeEmptyLine();

  // test_by_input(input_100_byte, 100, 10, 2, ip, port);
  // test_by_input(input_1_K, 100, 10, 2, ip, port);
  // test_by_input(input_10_K, 100, 10, 2, ip, port);
  // test_by_input(input_100_K, 100, 10, 2, ip, port);
  // test_by_input(input_1_M, 100, 10, 30, ip, port);
  // test_by_input(input_10_M, 10, 10, 200, ip, port);
}

void test_execute(Testor* testor, TestInput& input, int testSize, int parallel, float timeout, std::string ip, int port) {
  // testor->execute(testReport, testSize, input, parallel, ip, port,timeout);
  (*testor)(testReport, testSize, input, parallel, ip, port, timeout);
}

void test_by_testor(Testor* testor, int testSize, int parallel, std::string ip, int port) {
  // testor->execute(testReport, testSize, input, parallel, ip, port,timeout);
  (*testor)(testReport, testSize, input_100_byte, parallel, ip, port, 20);
  (*testor)(testReport, testSize, input_1_K, parallel, ip, port, 20);
  (*testor)(testReport, testSize, input_10_K, parallel, ip, port, 20);
  (*testor)(testReport, testSize, input_100_K, parallel, ip, port, 20);
  (*testor)(testReport, testSize, input_1_M, parallel, ip, port, 500);
  (*testor)(testReport, testSize, input_10_M, parallel, ip, port, 500);

  testReport.writeEmptyLine();
}

void test_by_input(TestInput& input, int testSize, int parallel, float timeout, std::string ip, int port) {
  test_sync(testReport, testSize, input, 1, ip, port, timeout);
  test_async(testReport, testSize, input, parallel, ip, port, timeout);
  test_long(testReport, testSize, input, parallel, ip, port, timeout);
  testReport.writeEmptyLine();
}