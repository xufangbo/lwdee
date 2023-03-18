#include "UhconnProtocol.h"
#include <stdio.h>
#include <string.h>
char  strHead[]="{[({";
char  strTail[]="})]}";
const std::string UhconnProtocol::left_enc =std::string(strHead);
const std::string UhconnProtocol::right_enc = std::string(strTail);
std::string UhconnProtocol::lastMsgStr=std::string("");
std::string UhconnProtocol::lastDataStr=std::string("");

UhconnProtocol::UhconnProtocol()
{

}

UhconnProtocol::~UhconnProtocol()
{

}

std::vector<std::string> UhconnProtocol::parseMsgBuff(char* buf, int len)
{
    std::vector<std::string> result;
    char *pp = buf;
    char *pEnd = buf+len-1;
    while(pp<pEnd){
        char *ph = strstr(pp,strHead);
        if(ph==NULL){
            char *pt = strstr(pp,strTail);
            if(pt!=NULL){
                result.push_back(lastMsgStr+std::string(pp,pt-pp));
                pp = pt+strlen(strTail);
            }else{
                return result; 
            }
        }else{
           char * pt = strstr(ph+strlen(strHead),strTail);
           if(pt != NULL){ 
               result.push_back(std::string(ph+strlen(strHead),pt-ph-strlen(strHead)));
               pp=pt+strlen(strTail);
           }else{
                lastMsgStr = std::string(ph+strlen(strHead),len-(ph-buf)-strlen(strHead));
                return result;
           }
        }
    }
    return result;
}


std::vector<std::string> UhconnProtocol::parseDataBuff(char* buf, int len)
{
    std::vector<std::string> result;
    char * pp = buf;
    char *pEnd = buf+len-1;
    while(pp<pEnd){
        char *ph = strstr(pp,strHead);
        if(ph==NULL){
            char *pt = strstr(pp,strTail);
            if(pt!=NULL){
                result.push_back(lastDataStr+std::string(pp,pt-pp));
                pp = pt+strlen(strTail);
            }else{
                return result; 
            }
        }else{
           char * pt = strstr(ph+strlen(strHead),strTail);
           if(pt != NULL){ 
               result.push_back(std::string(ph+strlen(strHead),pt-ph-strlen(strHead)));
               pp=pt+strlen(strTail);
           }else{
               lastDataStr = std::string(ph+strlen(strHead),len-(ph-buf)-strlen(strHead));
               return result;
           }
        }
    }
    return result;
}