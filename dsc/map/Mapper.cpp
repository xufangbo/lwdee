#include "Mapper.h"

#include <algorithm>
#include <regex>
#include <sstream>
#include "core/DscConfig.hpp"
#include "core/Stopwatch.h"
#include "core/log.hpp"
#include "lwdee/lwdee.h"
#include "map/MapDCO.h"

void Mapper::map(std::vector<std::string>* lines, vector<string>* words) {
  // 先合并成一行
  std::string output;
  for (std::string& line : *lines) {
    for (char& c : line) {      
      if (c >= 'A' && c <= 'Z') {
        output += (c + 32); // to lower
      } else if (c >= 'a' && c <= 'z') {
        output += c;
      } else {
        output += ' ';
      }
    }
  }

  // 执行分割
  stringstream iss(output);
  string token;
  while (!iss.eof()) {
    iss >> token;
    // token = trim(token);
    if (!token.empty()) {
      words->push_back(token);
    }
  }
}

// string trim(const string& str) {
//   string::size_type pos = str.find_first_not_of(' ');
//   if (pos == string::npos) {  // 若找不到空格
//     return str;
//   }
//   string::size_type pos2 = str.find_last_not_of(' ');
//   if (pos2 != string::npos)
//     return str.substr(pos, pos2 - pos + 1);  // 子串：第一个非空格作为起始位置
//                                              //  字符个数：pos2-pos+1
//   return str.substr(pos);
// }

// void Mapper::map(std::vector<std::string>& lines, vector<string>& words) {
//   // 先合并成一行
//   std::string line;
//   std::transform(line.begin(), line.end(), line.begin(), ::tolower);
//   for (auto& i : lines) {
//     line += " ";
//     line += i;
//   }

//   // 去掉数字
//   std::regex pattern1("[0-9]");
//   auto output1 = std::regex_replace(line, pattern1, "");

//   // 去掉特殊字符
//   std::regex pattern2("[,|-|.|?|:|-|”|“|!|;|’s]");
//   auto output2 = std::regex_replace(output1, pattern2, " ");

//   // 执行分割
//   stringstream iss(output2);
//   string token;
//   while (!iss.eof()) {
//     iss >> token;
//     token = trim(token);
//     if (!token.empty()) {
//       words.push_back(token);
//     }
//   }
// }