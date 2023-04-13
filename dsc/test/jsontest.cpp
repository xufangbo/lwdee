#include <exception>
#include <fstream>
#include <iostream>
#include "boost/json.hpp"
#include "core/cjson.hpp"

#define BUFSIZE 1024000

using namespace std;

std::string readFile() {
  std::string fileName = "/home/kevin/git/lwdee/doc/json.json";

  std::string txt;

  fstream f(fileName, ios_base::in);
  char buffer[BUFSIZE];
  while (!f.eof()) {
    f.getline(buffer, BUFSIZE);
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
  cout << "< boost json" << endl;
  std::string txt = readFile();
  boostTest(txt);

  cout << "> boost json parse ok" << endl;
}
