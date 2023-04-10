#ifndef UHCONNMSGPARSER_H
#define UHCONNMSGPARSER_H

#pragma once

#include "boost/json.hpp"
class UhconnMessage;
class UhconnMsgParser
{
public:
    UhconnMsgParser(std::string str);
    ~UhconnMsgParser();
    int parse(void);
    int getSrcNodeId();
    int getDestNodeId();
    int getCmd();
    int getType();
    std::string getClassName(void);
    std::string getMethodName(void);
    size_t  getMsgId(void);
    std::string  getFromDco(void);
    std::string  getToDco(void);
    unsigned long long  getDdoId(void);
    std::string  getMethodPara(void);
    int getMsg(UhconnMessage &msg);
private:
    std::string getString(char const *key);
    long long getInt(char const *key);
    unsigned long long getUint(char const*key);
    boost::json::object jobj;
    std::string jstr;
    bool objOK;
};

#endif