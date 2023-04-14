#include "UhconnProtocol.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "core/log.hpp"

// const char  UhconnProtocol::left_enc[]="{[({";
// const char  UhconnProtocol::right_enc[]="})]}";
const std::string UhconnProtocol::left_enc("{[({");
const std::string UhconnProtocol::right_enc("})]}");

UhconnProtocol::UhconnProtocol() : buffer("")
{
}

UhconnProtocol::~UhconnProtocol()
{

}

// std::vector<std::string> UhconnProtocol::parseMsgBuff(char* buf, int len)
// {
//     std::vector<std::string> result;
//     char *pp = buf;
//     char *pEnd = buf+len;
//     std::cout << "\n**********\n" << std::string(buf, len) << "\n**************" << std::endl; 
//     while(pp<pEnd){
//         char *ph = strstr(pp,left_enc);
//         if(ph==NULL){
//             char *pt = strstr(pp,right_enc);
//             if(pt!=NULL){
//                 // only }
//                 assert(lastMsgStr.find("{", 0) != std::string::npos);
//                 result.push_back(lastMsgStr+std::string(pp,pt-pp));
//                 lastMsgStr.clear();
//                 pp = pt+strlen(right_enc);
//             }else{
//                 //not {}
//                 assert(lastMsgStr.find("{", 0) != std::string::npos);
//                 lastMsgStr += std::string(buf,len);
//                 return result; 
//             }
//         }else{
//            char * pt = strstr(ph+strlen(left_enc),right_enc);
//            if(pt != NULL){
//                 // { } 
//                 assert(lastMsgStr.length() == 0);
//                 result.push_back(std::string(ph+strlen(left_enc),pt-ph-strlen(left_enc)));
//                 pp=pt+strlen(right_enc);
//            }else{
//                 // only {
//                 assert(lastMsgStr.length() == 0);
//                 lastMsgStr = std::string(ph+strlen(left_enc),len-(ph-buf)-strlen(left_enc));
//                 return result;
//            }
//         }
//     }
//     return result;
// }
std::vector<std::string> UhconnProtocol::parseMsgBuff(char* buf, int len)
{
    std::vector<std::string> result;
    std::string newdata(buf, len); 
    logger_trace("%s",newdata.c_str());
    buffer += newdata; // 将新数据追加到缓存中

    size_t start_pos = 0;
    while (true) {
        size_t left_pos = buffer.find(left_enc, start_pos);
        if (left_pos == std::string::npos) {
            break; // 缓存中不存在左分隔符，退出循环
        }

        size_t right_pos = buffer.find(right_enc, left_pos + left_enc.length());
        if (right_pos == std::string::npos) {
            break; // 缓存中不存在右分隔符，退出循环
        }

        // 找到一帧数据，加入结果vector
        result.emplace_back(buffer.substr(left_pos + left_enc.length(),
                                           right_pos - left_pos - left_enc.length()));

        // 更新下一轮搜索的起始位置
        start_pos = right_pos + right_enc.length();
    }

    // 更新缓存，保留未处理的部分数据
    buffer = buffer.substr(start_pos);

    logger_trace("%s",buffer.c_str());
    return result;
}


// std::vector<std::string> UhconnProtocol::parseDataBuff(char* buf, int len)
// {
//     std::vector<std::string> result;
//     char * pp = buf;
//     char *pEnd = buf+len-1;
//     while(pp<pEnd){
//         char *ph = strstr(pp,left_enc);
//         if(ph==NULL){
//             char *pt = strstr(pp,right_enc);
//             if(pt!=NULL){
//                 result.push_back(lastDataStr+std::string(pp,pt-pp));
//                 pp = pt+strlen(right_enc);
//             }else{
//                 return result; 
//             }
//         }else{
//            char * pt = strstr(ph+strlen(left_enc),right_enc);
//            if(pt != NULL){ 
//                result.push_back(std::string(ph+strlen(left_enc),pt-ph-strlen(left_enc)));
//                pp=pt+strlen(right_enc);
//            }else{
//                lastDataStr = std::string(ph+strlen(left_enc),len-(ph-buf)-strlen(left_enc));
//                return result;
//            }
//         }
//     }
//     return result;
// }