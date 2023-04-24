#include "Qps.hpp"

void Qps::reset() {
  sockets = 0;
  closes = 0;
  recvs = 0;
  sends = 0;
  bullets = 0;

  sockets_ = 0;
  closes_ = 0;
  recvs_ = 0;
  sends_ = 0;
  bullets_ = 0;
}

std::vector<std::string> Qps::header() {
  std::vector<std::string> ss;
  ss.push_back("skts");
  ss.push_back("recvs");
  ss.push_back("sends");
  ss.push_back("blts");
  ss.push_back("clses");
  return ss;
}

void Qps::generate() {
  values.clear();
  values.push_back(sockets - sockets_);
  sockets_ = sockets;
  values.push_back(recvs - recvs_);
  recvs_ = recvs;
  values.push_back(sends - sends_);
  sends_ = sends;
  values.push_back(bullets - bullets_);
  bullets_ = bullets;
  values.push_back(closes - closes_);
  closes_ = closes;
}

std::vector<std::string> Qps::data() {
  std::vector<std::string> ss;

  for (auto& i : values) {
    ss.push_back(std::to_string(i));
  }

  return ss;
}