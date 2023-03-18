#include "UhconnTCPServer.h"
#include "libgo.h"

UhconnTCPServer::UhconnTCPServer()
{
}

UhconnTCPServer::~UhconnTCPServer()
{
	detach();
}

string UhconnTCPServer::message;

void UhconnTCPServer::setup(int port)
{
	sockfd=socket(AF_INET,SOCK_STREAM,0);
 	memset(&serverAddress,0,sizeof(serverAddress));
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddress.sin_port=htons(port);
	bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));
 	listen(sockfd,5);
    go [&]{
		// cout << "router start to rec.." << endl;
		this->receive();
    };
}

void UhconnTCPServer::receive()
{
	while(1)
	{
		socklen_t sosize  = sizeof(clientAddress);
		int *tmpsockfd = new int;
		*tmpsockfd = accept(sockfd,(struct sockaddr*)&clientAddress,&sosize);
		go [=]{
			char * msg =(char*) malloc(MAXPACKETSIZE);
			memset(msg,0,MAXPACKETSIZE);
			int *ret = new int;
		    while(1)
			{
				*ret=recv(*tmpsockfd, msg, MAXPACKETSIZE-1, 0);
				if(*ret==0 || *ret < 0)
				{
					close(*tmpsockfd);
					break;
				}
				// msg[*ret]=0;
				// message = string(msg);
				// if(message.length()>0){
				// 	#ifdef DEBUGINFO
				// 	cout << "rx msg str:" << message << endl;
				// 	#endif
				// 	this->handler(this->param, msg,*tmpsockfd);
				// }
				this->handler(this->param, (void*)msg,*tmpsockfd,*ret);
			}
			close(*tmpsockfd);
			delete tmpsockfd;
			delete ret;
			free(msg);
			return;
		};
	}
}

string UhconnTCPServer::getMessage()
{
	return message;
}

int UhconnTCPServer::Send(string msg)
{
	return send(newsockfd,msg.c_str(),msg.length(),0);
}

int UhconnTCPServer::Send(unsigned char * buf, int len, int sockFd)
{
	return send(sockFd,buf,len,0);
}

void UhconnTCPServer::clean()
{
	message = "";
	memset(msg, 0, MAXPACKETSIZE);
}

void UhconnTCPServer::detach()
{
	close(sockfd);
	close(newsockfd);
} 

void UhconnTCPServer::setRxHandler(rx_handler h, void * p){
	handler = h;
	param = p;
}