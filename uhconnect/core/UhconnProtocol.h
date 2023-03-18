#ifndef UHCONNPROTOCOL_H
#define UHCONNPROTOCOL_H

#pragma once
#include <string>
#include <iostream>

#include <vector>
class UhconnProtocol
{
public:
    UhconnProtocol();
    ~UhconnProtocol();
    static std::vector<std::string> parseMsgBuff(char* buf, int len);
    static std::vector<std::string> parseDataBuff(char* buf, int len);
    static const std::string left_enc; 
    static const std::string right_enc; 
private:
    static std::string lastMsgStr;
    static std::string lastDataStr;
};

#endif