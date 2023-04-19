#include "Qps.hpp"

void Qps::reset() {
  opens = 0;
  closes = 0;
  inputs = 0;
  outputs = 0;
  trans_min = 100000000;
  trans_sum = 0;
  trans_max = 0;
}

void Qps::time(uint32_t elapsed) {
  trans_min = trans_min < elapsed ? trans_min.load() : elapsed;
  trans_max = trans_max > elapsed ? trans_max.load() : elapsed;
  trans_sum += elapsed;
}

std::vector<std::string> Qps::header() {
  std::vector<std::string> ss;
  ss.push_back("opens");
  ss.push_back("inputs");
  ss.push_back("outputs");
  ss.push_back("closes");
  ss.push_back("waitings");
  return ss;
}

std::vector<std::string> Qps::data() {
  std::vector<std::string> ss;
  ss.push_back(std::to_string(opens));
  ss.push_back(std::to_string(outputs));
  ss.push_back(std::to_string(inputs));
  ss.push_back(std::to_string(closes));
  ss.push_back(std::to_string(waitings()));
  return ss;
}