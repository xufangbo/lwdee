#include "UhconnConfig.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>

UhconnConfig::UhconnConfig()
{
    isConfigured = false;
    confNodeAmount = 0;
    nodeAmount =0;
}

UhconnConfig::~UhconnConfig()
{

}

int UhconnConfig::loadConf(std::string file, std::string name){
 std::ifstream file_stream(file, std::ios_base::in);
  if (!file_stream) {
    std::cout << "Could not open file " << file << std::endl;
    return -1;
  }

  std::stringstream ss;
  boost::json::object allConf;
  ss << file_stream.rdbuf();
  std::string jsonTxt(ss.str());

  // std::cout << "config file:"<<jsonTxt; 
  auto obj = boost::json::parse(jsonTxt);
  try{
      allConf =obj.as_object();
  }catch(std::exception &e){
      std::cout << "exception:" << e.what()<<std::endl;
      return -2;
  }

  if(allConf.contains(name)){
    auto c = allConf.at(name);
    jconf = c.as_object();
    isConfigured = true;
  }else{
    std::cout << name << " conf not found!!"<<std::endl;
    return -3;
  }

  if(allConf.contains("node_amount")){
    auto c = allConf.at("node_amount");
    confNodeAmount = c.as_int64();
    nodeAmount = confNodeAmount;
  }else{
    std::cout << "node_amount conf not found!!"<<std::endl;
    return -4;
  }

  std::stringstream cs;
  cs << boost::json::serialize(jconf) << std::endl;
  std::cout << name << " conf:"<<cs.str()<<std::endl;
  std::cout << "node amount:"<<confNodeAmount<<std::endl;
  return 0;
}

long long UhconnConfig::getInt(char const *key){
    if(!isConfigured){
        return -1;
    }
    if(!jconf.contains(key)){
        return -2;
    }
    auto id = jconf.at(key);
    return id.as_int64();
}

std::string UhconnConfig::getString(char const *key){
    if(!isConfigured){
        return std::string("");
    }
    if(!jconf.contains(key)){
        return std::string("");
    }
    auto str = jconf.at(key);
    return std::string(str.as_string().c_str());
}

std::string UhconnConfig::getIpAddr(){
    return getString("ip");
}

int UhconnConfig::getNodeId(){
  return getInt("nodeId");
}

int UhconnConfig::getMsgPort(){
 return getInt("msgPort");
}
int UhconnConfig::getDataPort(){
  return getInt("dataPort");
}

int UhconnConfig::getRouteInfo(std::map<int,NodeInfo> &map_info){
  if(!isConfigured){
      std::cout<<"config not done while getRouteInfo"<<std::endl;
      return -1;
    }
    if(!jconf.contains("routeInfo")){
      std::cout<<"routeInfo not found in configure object"<<std::endl;
        return -2;
    }
    auto obj_info = jconf.at("routeInfo");
    auto array = obj_info.as_array();
   // std::map<int,NodeInfo> map_info;
    for(auto item: array){
      NodeInfo i;
      i.dataPort = item.at("dport").as_int64();
      i.ipv4Addr = std::string(item.at("ip").as_string().c_str());
      i.msgPort = item.at("mport").as_int64();
      i.nodeId = item.at("nid").as_int64();
      i.dataClient = NULL;
      i.msgClient = NULL;
      i.msgLock = new co_mutex;
      i.dataLock = new co_mutex;
      map_info.insert(std::pair<int,NodeInfo>(i.nodeId, i));
    }
    return 0;
}

int UhconnConfig::getNodeAmount(void){
  return nodeAmount; 
}

int UhconnConfig::setNodeAmount(int amount){
  nodeAmount = amount;
  std::cout<<"setNodeAmount to "<<nodeAmount<<std::endl;
  return 0;
}