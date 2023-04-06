#include "Mapper.h"

#include <regex>
#include <sstream>
#include "core/Stopwatch.h"
#include "core/log.hpp"
#include "lwdee/lwdee.h"
#include "map/MapDCO.h"

void Mapper::map(std::vector<std::string>& lines) {
  std::string line;
  for (auto& i : lines) {
    line += " ";
    line += i;
  }

  auto words = split(line);
}

std::vector<std::string> Mapper::split(std::string& line) {
  //
  std::regex pattern1("[0-9]");
  auto output1 = std::regex_replace(line, pattern1, "");

  //
  std::regex pattern2("[,|-|.|?|:|-|”|“|!|;|’s]");
  auto output2 = std::regex_replace(output1, pattern2, " ");

  //

  vector<string> outpus;
  stringstream iss(output2);
  string token;
  while (!iss.eof()) {
    iss >> token;
    if (!token.empty()) {
      outpus.push_back(token);
    }
  }

  return outpus;
}