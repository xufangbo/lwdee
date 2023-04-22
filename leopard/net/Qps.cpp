#include "Qps.hpp"

void Qps::reset() {
  opens = 0;
  closes = 0;
  recvs = 0;
  sends = 0;
  bullets = 0;
  sockets = 0;
}

std::vector<std::string> Qps::header() {
  std::vector<std::string> ss;
  ss.push_back("OPENS");
  ss.push_back("recvs");
  ss.push_back("sends");
  ss.push_back("closes");
  ss.push_back("bullets");
  ss.push_back("sockets");
  return ss;
}

std::vector<std::string> Qps::data() {
  if (waiting_fun != nullptr) {
    sockets = waiting_fun();
  }

  std::vector<std::string> ss;
  ss.push_back(std::to_string(opens));
  ss.push_back(std::to_string(sends));
  ss.push_back(std::to_string(recvs));
  ss.push_back(std::to_string(closes));
  ss.push_back(std::to_string(bullets));
  ss.push_back(std::to_string(sockets));
  return ss;
}