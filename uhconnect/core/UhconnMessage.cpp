#include "UhconnMessage.h"
#include "boost/json.hpp"
#include <sstream>

using namespace std;
UhconnMessage::UhconnMessage() {

}

UhconnMessage::~UhconnMessage()
{

}


int UhconnMessage::getSrcNodeId() const{
    return srcNodeId;
}

int UhconnMessage::getDestNodeId() const{
    return destNodeId;
}

string UhconnMessage::getMessageJsonStr() {
    boost::json::object jobj;

    jobj["srcNodeId"] = srcNodeId;
    jobj["destNodeId"] = destNodeId;
    if(fromDco.length()){
       jobj["fromDco"] = fromDco;
    }
    if(toDco.length()){
       jobj["toDco"] = toDco;
    }
    jobj["msgId"] = msgId;
    jobj["cmd"] = cmd;
    jobj["type"] = type;
    if(className.length()){
        jobj["className"] =className;
    }
    if(methodName.length()){
        jobj["methodName"] = methodName;
    }
    if(methodPara.length()){
        jobj["methodPara"] = methodPara;
    }    
    if(DdoId != 0){
        std::stringstream ss;
        ss << DdoId;
        jobj["ddoId"] =  ss.str();
    }

    addMessageExtendJsonItem(jobj);
    std::stringstream jss;
    jss << boost::json::serialize(jobj) << endl;
    std::string str = jss.str();

    return str;
}

void UhconnMessage::setClassName(std::string name){
    className = name;
}

void UhconnMessage::setMsgId(unsigned int id){
    msgId = id;
}

