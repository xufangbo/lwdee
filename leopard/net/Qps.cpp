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
  ss.push_back("sockets");
  ss.push_back("recvs");
  ss.push_back("sends");
  ss.push_back("bullets");
  ss.push_back("closes");  
  return ss;
}

std::vector<std::string> Qps::data() {
  std::vector<std::string> ss;
  ss.push_back(std::to_string(sockets));
  ss.push_back(std::to_string(sends));
  ss.push_back(std::to_string(recvs));
  ss.push_back(std::to_string(bullets));
  ss.push_back(std::to_string(closes));
  return ss;
}