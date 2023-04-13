#include "Mapper.h"

#include <algorithm>
#include <regex>
#include <sstream>
#include "core/DscConfig.hpp"
#include "core/Stopwatch.h"
#include "core/cjson.hpp"
#include "core/log.hpp"
#include "lwdee/lwdee.h"
#include "map/MapDCO.h"

void Mapper::map(std::vector<std::string>* lines, vector<DeviceRecord>* words) {
  for (std::string& line : *lines) {
    DeviceRecord word;
    if (word.fromJson(&line)) {
      words->push_back(word);
      // logger_trace("did:%s,ts:%d", word.did.c_str(), word.ts);
    }
  }
}

// void Mapper::map(std::vector<std::string>* lines, vector<string>* words) {
//   // 先合并成一行
//   std::string output;
//   for (std::string& line : *lines) {
//     for (char& c : line) {
//       if (c >= 'A' && c <= 'Z') {
//         output += (c + 32); // to lower
//       } else if (c >= 'a' && c <= 'z') {
//         output += c;
//       } else {
//         output += ' ';
//       }
//     }
//   }

//   // 执行分割
//   stringstream iss(output);
//   string token;
//   while (!iss.eof()) {
//     iss >> token;
//     // token = trim(token);
//     if (!token.empty()) {
//       words->push_back(token);
//     }
//   }
// }