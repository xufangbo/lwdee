#ifndef UHCONNTCPCLIENT_H
#define UHCONNTCPCLIENT_H

#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>

using namespace std;
class UhconnTCPClient
{
public:
    UhconnTCPClient();
    ~UhconnTCPClient();
    bool setup(string address, int port);
    bool Send(const string& data);
    int receive(unsigned char * buf, int size);
    int getSockFd(){return sock;}
    void exit();

private:
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;
};

#endif