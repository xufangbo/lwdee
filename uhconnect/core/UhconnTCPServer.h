#ifndef UHCONNTCPSERVER_H
#define UHCONNTCPSERVER_H
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <functional>

#pragma once

using namespace std;
#define MAXPACKETSIZE  128*1024

// typedef int(*rx_handler)(void* p1, void *p2, int fd, int len);
using rxFunction = std::function<int(char*, int, int)>;

class UhconnRouter;
class UhconnTCPServer
{
	UhconnTCPServer();
public:
    UhconnTCPServer(int port, rxFunction rxHandlerFunc);
    ~UhconnTCPServer();

    int sockfd, newsockfd, n, pid;
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;
	pthread_t serverThread;
	// char msg[ MAXPACKETSIZE ];
	// static string message;


	void receive();
	// string getMessage();
	// int Send(string msg);
	static int Send(const char * buf, int len, int sockFd);
	void detach();
	void clean();
	// void setRxHandler(rx_handler h, void * p);
private:
	void setup(int port);
    rxFunction rxHandler; // 保存成员函数指针;
	enum {
		ok = 0,
		err_fd = -1,
		err_send = -2,
	};
};

#endif