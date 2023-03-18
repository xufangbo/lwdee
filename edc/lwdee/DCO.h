#pragma once

// #include "api/UhconnDdoRef.h"
// #include <iostream>
#include "DDO.h"

class DCO {
 private:
  std::string className;

 public:
  DCO(std::string clsname) : className(clsname){};
  void async(std::string function);
  DDO wait();
};