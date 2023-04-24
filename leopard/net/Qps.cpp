#include "Qps.hpp"

void Qps::reset() {
  sockets = 0;
  closes = 0;
  recvs = 0;
  sends = 0;
  bullets = 0;
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

std::vector<std::string> Qps::data() {
  std::vector<std::string> ss;

  ss.push_back(std::to_string(sockets - sockets_));
  sockets_ = sockets;
  ss.push_back(std::to_string(recvs - recvs_));
  recvs_ = recvs;
  ss.push_back(std::to_string(sends - sends_));
  sends_ = sends;
  ss.push_back(std::to_string(bullets - bullets_));
  bullets_ = bullets;
  ss.push_back(std::to_string(closes - closes_));
  closes_ = closes;
  return ss;
}