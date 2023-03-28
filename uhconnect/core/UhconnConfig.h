#ifndef UHCONNCONFIG_H
#define UHCONNCONFIG_H

#pragma once

#include <boost/json.hpp>
#include <map>
#include "UhconnRouter.h"

class UhconnConfig {
 public:
  UhconnConfig();
  ~UhconnConfig();
  int loadConf(std::string file, std::string name);
  int getNodeId();
  int getMsgPort();
  int getDataPort();
  std::string getIpAddr();
  int getNodeAmount(void);
  int setNodeAmount(int amount);
  int getRouteInfo(std::map<int, NodeInfo>& map_info);
  static UhconnConfig& getInstance(void) {
    static UhconnConfig instance;
    return instance;
  };

 private:
  long long getInt(char const* key);
  std::string getString(char const* key);
  boost::json::object jconf;
  int confNodeAmount;
  int nodeAmount;
  bool isConfigured;
};

#endif