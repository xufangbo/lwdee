#include "UhconnMsgParser.h"
#include <iostream>
#include <sstream>
// #include <mutex>
#include "UhconnMessage.h"
// #include "core/cjson.hpp"

using namespace std;

UhconnMsgParser::UhconnMsgParser(std::string str) {
  jstr = str;
  objOK = false;
}

UhconnMsgParser::~UhconnMsgParser() {
}

// void cjson_test(std::string &json){
//   cJSON* node = cJSON_Parse(json.c_str());
//   if(node == NULL){
//     cout << "can't parse json string " << endl;
//     return;
//   }

//   cout << "srcNodeId  : " << cJSON_GetObjectItem(node, "srcNodeId")->valueint << endl;
//   cout << "destNodeId : " << cJSON_GetObjectItem(node, "destNodeId")->valueint << endl;
//   cout << "fromDco    : " << cJSON_GetObjectItem(node, "fromDco")->valuestring << endl;
//   cout << "toDco      : " << cJSON_GetObjectItem(node, "toDco")->valuestring << endl;
//   cout << "cmd        : " << cJSON_GetObjectItem(node, "cmd")->valueint << endl;
//   cout << "type       : " << cJSON_GetObjectItem(node, "type")->valueint << endl;
//   cout << "methodName : " << cJSON_GetObjectItem(node, "methodName")->valuestring << endl;
//   cout << "ddoId      : " << cJSON_GetObjectItem(node, "ddoId")->valuestring << endl;

//   cJSON * items = cJSON_GetObjectItem(node, "methodPara");
//   int size = cJSON_GetArraySize(items);
//   for(int i=0;i<size;i++){
//     cout << cJSON_GetArrayItem(items,i)->valuestring  << " ";
//   }
//   cout << endl;
// }

// std::mutex mtx;

int UhconnMsgParser::parse(void) {
  // mtx.lock();
  try {    
    auto pjo = boost::json::parse(jstr);
    jobj = pjo.as_object();
  } catch (exception& e) {
    cout << "exception:" << e.what() << endl;
    std::cout << jstr << std::endl;
    objOK = false;

    // cjson_test(jstr);

    return -1;
  }
  objOK = true;

  // mtx.unlock();
  return 0;
}

long long UhconnMsgParser::getInt(char const* key) {
  if (!objOK) {
    return -1;
  }
  if (!jobj.contains(key)) {
    return -2;
  }
  auto id = jobj.at(key);
  return id.as_int64();
}

unsigned long long UhconnMsgParser::getUint(char const* key) {
  if (!objOK) {
    return -1;
  }
  if (!jobj.contains(key)) {
    return -2;
  }
  auto id = jobj.at(key);
  return id.as_uint64();
}

std::string UhconnMsgParser::getString(char const* key) {
  if (!objOK) {
    return std::string("");
  }
  if (!jobj.contains(key)) {
    return std::string("");
  }
  auto str = jobj.at(key);
  return std::string(str.as_string().c_str());
}

int UhconnMsgParser::getSrcNodeId() {
  return getInt("srcNodeId");
}

int UhconnMsgParser::getCmd() {
  return getInt("cmd");
}

int UhconnMsgParser::getType() {
  return getInt("type");
}

int UhconnMsgParser::getDestNodeId() {
  return getInt("destNodeId");
}

std::string UhconnMsgParser::getClassName(void) {
  return getString("className");
}

std::string UhconnMsgParser::getMethodName(void) {
  return getString("methodName");
}

size_t UhconnMsgParser::getMsgId(void) {
  std::stringstream ss(getString("msgId"));
  size_t result = 0;
  ss >> result;
  return result;
}

std::string UhconnMsgParser::getFromDco(void) {
  return getString("fromDco");
}

std::string UhconnMsgParser::getToDco(void) {
  return getString("toDco");
}

unsigned long long UhconnMsgParser::getDdoId(void) {
  std::string str = getString("ddoId");
  std::stringstream ss(str);
  unsigned long long result = 0;
  ss >> result;
  // std::cout << "UhconnMsgParser::getDdoId() str: "<<str<<" ddoid:"<< result<<std::endl;
  return result;
}

std::string UhconnMsgParser::getMethodPara(void) {
  return getString("methodPara");
}

int UhconnMsgParser::getMsg(UhconnMessage& msg) {
  int srcNode = getSrcNodeId();
  int destNode = getDestNodeId();
  int cmd = getCmd();
  int type = getType();
  msg.setupMessage(srcNode, destNode, (Uh_Message_Command_E)cmd, (Uh_Message_Type_E)type);
  msg.setClassName(getClassName());
  msg.setDdoId(getDdoId());
  msg.setMethodName(getMethodName());
  msg.setMethodPara(getMethodPara());
  msg.setDestVoxor(getToDco());
  msg.setSrcVoxor(getFromDco());
  msg.setMsgId(getMsgId());
  return 0;
}