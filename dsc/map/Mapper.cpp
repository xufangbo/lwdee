#include "Mapper.h"

#include <algorithm>
#include <regex>
#include <sstream>
#include "core/DscConfig.hpp"
#include "core/Stopwatch.h"
#include "core/log.hpp"
#include "lwdee/lwdee.h"
#include "map/MapDCO.h"

void Mapper::map(std::vector<std::string>& lines, vector<string>& words) {
  // 先合并成一行
  std::string line;
  std::transform(line.begin(), line.end(), line.begin(), ::tolower);
  for (auto& i : lines) {
    line += " ";
    line += i;
  }

  // 去掉数字
  std::regex pattern1("[0-9]");
  auto output1 = std::regex_replace(line, pattern1, "");

  // 去掉特殊字符
  std::regex pattern2("[,|-|.|?|:|-|”|“|!|;|’s]");
  auto output2 = std::regex_replace(output1, pattern2, " ");

  // 执行分割
  stringstream iss(output2);
  string token;
  while (!iss.eof()) {
    iss >> token;
    if (!token.empty()) {
      words.push_back(token);
    }
  }
}