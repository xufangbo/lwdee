#include <exception>
#include <fstream>
#include <iostream>
#include "boost/json.hpp"
#include "core/cjson.hpp"

using namespace std;

std::string readFile() {
  std::string fileName = "/home/kevin/git/lwdee/doc/json.txt";

  std::string txt;

  fstream f(fileName, ios_base::in);
  char buffer[1024];
  while (!f.eof()) {
    f.getline(buffer, 1024);
    txt += buffer;
  }
  return txt;
}

void boostTest(std::string txt) {
  try {
    auto pjo = boost::json::parse(txt);
    auto jobj = pjo.as_object();
    cout << "boost::json ok" << endl;
  } catch (exception& e) {
    cout << "exception:" << e.what() << endl;
  }
}

void jsonTest() {
  std::string txt = readFile();
  boostTest(txt);
}
