#include "UhconnMsgParser.h"
#include <iostream>
#include "UhconnMessage.h"
#include <sstream>

using namespace std;

UhconnMsgParser::UhconnMsgParser(std::string str)
{
    jstr = str;
    objOK =false;
}

UhconnMsgParser::~UhconnMsgParser()
{

}

int UhconnMsgParser::parse(void){
    // std::cout<<"parse JSON str:"<<jstr<<std::endl;
    try{
        auto pjo = boost::json::parse(jstr);
        jobj =pjo.as_object();
    }catch(exception &e){
        cout << "exception:" << e.what()<<endl;
        objOK = false;
        return -1;
    }
    objOK = true;
    return 0;
}   

long long UhconnMsgParser::getInt(char const*key){
    if(!objOK){
        return -1;
    }
    if(!jobj.contains(key)){
        return -2;
    }
    auto id = jobj.at(key);
    return id.as_int64();
}

unsigned long long UhconnMsgParser::getUint(char const*key){
    if(!objOK){
        return -1;
    }
    if(!jobj.contains(key)){
        return -2;
    }
    auto id = jobj.at(key);
    return id.as_uint64();
}

std::string UhconnMsgParser::getString(char const*key){
    if(!objOK){
        return std::string("");
    }
    if(!jobj.contains(key)){
        return std::string("");
    }
    auto str = jobj.at(key);
    return std::string(str.as_string().c_str());
}

int UhconnMsgParser::getSrcNodeId(){
    return getInt("srcNodeId");
}

int UhconnMsgParser::getCmd(){
    return getInt("cmd");
}

int UhconnMsgParser::getType(){
    return getInt("type");
}

int UhconnMsgParser::getDestNodeId(){
    return getInt("destNodeId");
}

std::string UhconnMsgParser::getClassName(void){
    return getString("className");
}

std::string UhconnMsgParser::getMethodName(void){
    return getString("methodName");
}

size_t  UhconnMsgParser::getMsgId(void){
    std::stringstream ss(getString("msgId"));
    size_t result = 0;
    ss >> result;
    return result;
}

std::string  UhconnMsgParser::getFromDco(void){
    return getString("fromDco");
}

std::string  UhconnMsgParser::getToDco(void){
    return getString("toDco");
}

unsigned long long  UhconnMsgParser::getDdoId(void){
    std::string str =  getString("ddoId");
    std::stringstream ss(str);
    unsigned long long result=0;
    ss>>result;
   // std::cout << "UhconnMsgParser::getDdoId() str: "<<str<<" ddoid:"<< result<<std::endl;
    return result;

}

std::string  UhconnMsgParser::getMethodPara(void){
    return getString("methodPara");

}

int UhconnMsgParser::getMsg(UhconnMessage &msg){
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