#ifndef UHCONNMESSAGE_H
#define UHCONNMESSAGE_H

#pragma once

#include <string>
#include "boost/json.hpp"

typedef enum {
    MSG_CMD_CREATE,
    MSG_CMD_WAIT,
    MSG_CMD_ASYNC,
}Uh_Message_Command_E;

typedef enum {
    MSG_TYPE_REQ,
    MSG_TYPE_RESP,
}Uh_Message_Type_E;



class UhconnMessage
{
public:
    UhconnMessage();
    UhconnMessage(int src_node, int dest_node, Uh_Message_Command_E msg_cmd, Uh_Message_Type_E msg_type) {
        srcNodeId = src_node;
        destNodeId = dest_node;
        cmd = msg_cmd;
        type = msg_type;
        DdoId = 0;
        msgId =0;
    };
    virtual ~UhconnMessage();
    std::string getMessageJsonStr();
    virtual void addMessageExtendJsonItem(boost::json::object& base) {};
    void setupMessage(int src_node, int dest_node, Uh_Message_Command_E msg_cmd, Uh_Message_Type_E msg_type) {
        srcNodeId = src_node;
        destNodeId = dest_node;
        cmd = msg_cmd;
        type = msg_type;
        DdoId = 0;
        msgId =0;
    };
    int getSrcNodeId() const;
    int getDestNodeId() const;
    void setClassName(std::string name);
    void setMsgId(unsigned int id);

    void setDdoData(unsigned int dt);

    void setDdoId(unsigned long long id) {DdoId = id;};
    unsigned long long getDdoId() const {return DdoId;};

    void setMethodName(const std::string& name) {methodName = name;};
    void setMethodPara(const std::string& para) {methodPara = para;};
    const std::string& getMethodName() const{return methodName;};
    const std::string& getMethodPara() const{return methodPara;};

    const std::string& getDestVoxor() const {return toDco;};
    const std::string& getSrcVoxor() const {return fromDco;};
    void setDestVoxor(const std::string& to) {toDco = to;};
    void setSrcVoxor(const std::string& from) {fromDco = from;};

    Uh_Message_Command_E getCmd(void) const {return cmd;};
    Uh_Message_Type_E getType(void) const {return type;};
private:
    int srcNodeId;
    int destNodeId;
    std::string fromDco; 
    std::string toDco;   
    unsigned int msgId;
    unsigned long long DdoId;

    std::string fromVoxorId;
    std::string toVoxorId;
    Uh_Message_Command_E cmd;
    Uh_Message_Type_E type;
    std::string className;
    std::string methodName;
    std::string methodPara;
};

#endif