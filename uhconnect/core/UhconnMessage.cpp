#include "UhconnMessage.h"
#include "boost/json.hpp"
#include <boost/lexical_cast.hpp>
#include <sstream>

using namespace std;
UhconnMessage::UhconnMessage() {
    type = MSG_TYPE_REQ;
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

// string UhconnMessage::getMessageJsonStr() {
//     boost::json::value jobj(boost::json::object{
//         {"srcNodeId", srcNodeId},
//         {"destNodeId", destNodeId},
//         {"cmd", boost::lexical_cast<int>(cmd)},
//         {"type", boost::lexical_cast<int>(type)}
//     });

//     if (!fromDco.empty()) {
//         jobj.as_object().emplace("fromDco", fromDco);
//     }
//     if (!toDco.empty()) {
//         jobj.as_object().emplace("toDco", toDco);
//     }
//     if (msgId != 0) {
//         jobj.as_object().emplace("msgId", boost::lexical_cast<string>(msgId));
//     }
//     if (!className.empty()) {
//         jobj.as_object().emplace("className", className);
//     }
//     if (!methodName.empty()) {
//         jobj.as_object().emplace("methodName", methodName);
//     }
//     if (!methodPara.empty()) {
//         jobj.as_object().emplace("methodPara", methodPara);
//     }    
//     if (DdoId != 0) {
//         jobj.as_object().emplace("ddoId", boost::lexical_cast<string>(DdoId));
//     }

//     addMessageExtendJsonItem(jobj.as_object());

//     return boost::json::serialize(jobj);
// }

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
    if(msgId != 0){
        std::stringstream ssmsgId;
        ssmsgId << msgId;
        jobj["msgId"] =  ssmsgId.str();
    }
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

    // addMessageExtendJsonItem(jobj);
    // std::stringstream jss;
    // jss << boost::json::serialize(jobj) << endl;
    // std::string str = jss.str();

    // return str;
    return boost::json::serialize(jobj);
}

void UhconnMessage::setClassName(std::string name){
    className = name;
}

void UhconnMessage::setMsgId(size_t id){
    msgId = id;
}

std::atomic<int> MessagIdHelper::seq(0);
