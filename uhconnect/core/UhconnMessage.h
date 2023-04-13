#ifndef UHCONNMESSAGE_H
#define UHCONNMESSAGE_H

#pragma once

#include <string>
#include "boost/json.hpp"

typedef enum {
    MSG_CMD_CREATE,
    MSG_CMD_WAIT,
    MSG_CMD_ASYNC,
    MSG_CMD_ASYNC_PRO,
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
    std::string getClassName(void) const {
        return className;
    };
    void setMsgId(size_t id);
    size_t getMsgId(void) const {
        return msgId;
    };

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
    size_t msgId;
    unsigned long long DdoId;

    std::string fromVoxorId;
    std::string toVoxorId;
    Uh_Message_Command_E cmd;
    Uh_Message_Type_E type;
    std::string className;
    std::string methodName;
    std::string methodPara;
};

namespace std {
    template <>
    struct hash<UhconnMessage> {
        size_t operator()(const UhconnMessage& msg) const {
            size_t h1 = std::hash<int>()(msg.getSrcNodeId());
            size_t h2 = std::hash<int>()(msg.getDestNodeId());
            size_t h3 = std::hash<int>()(static_cast<int>(msg.getCmd()));
            size_t h4 = std::hash<int>()(static_cast<int>(msg.getType()));
            size_t h5 = 0;//std::hash<std::string>()(msg.getMessageJsonStr());
            size_t h6 = std::hash<size_t>()(msg.getMsgId());
            size_t h7 = std::hash<unsigned long long>()(msg.getDdoId());
            size_t h8 = std::hash<std::string>()(msg.getDestVoxor());
            size_t h9 = std::hash<std::string>()(msg.getSrcVoxor());
            size_t h10 = std::hash<std::string>()(msg.getClassName());
            size_t h11 = std::hash<std::string>()(msg.getMethodName());
            size_t h12 = std::hash<std::string>()(msg.getMethodPara());
            return h1 ^ h2 ^ h3 ^ h4 ^ h5 ^ h6 ^ h7 ^ h8 ^ h9 ^ h10 ^ h11 ^ h12;
        }
    };
}
#endif