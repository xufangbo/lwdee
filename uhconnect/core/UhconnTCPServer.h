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

#pragma once

using namespace std;
#define MAXPACKETSIZE  128*1024

typedef int(*rx_handler)(void* p1, void *p2, int fd, int len);

class UhconnTCPServer
{
public:
    UhconnTCPServer();
    ~UhconnTCPServer();

    int sockfd, newsockfd, n, pid;
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;
	pthread_t serverThread;
	char msg[ MAXPACKETSIZE ];
	static string message;

	void setup(int port);
	void receive();
	string getMessage();
	int Send(string msg);
	int Send(unsigned char * buf, int len, int sockFd);
	void detach();
	void clean();
	void setRxHandler(rx_handler h, void * p);
private:
    rx_handler handler;
	void* param;
};

#endif