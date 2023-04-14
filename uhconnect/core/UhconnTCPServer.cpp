#include "UhconnTCPServer.h"
#include "libgo.h"
#include "UhconnRouter.h"

UhconnTCPServer::UhconnTCPServer()
{
}

UhconnTCPServer::UhconnTCPServer(int port, rxFunction rxHandlerFunc) : rxHandler(rxHandlerFunc) {
	setup(port);
}

UhconnTCPServer::~UhconnTCPServer()
{
	detach();
}

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
			int ret = 0;
		    while(1)
			{
				ret=recv(*tmpsockfd, msg, MAXPACKETSIZE-1, 0);
				if(ret==0 || ret < 0)
				{
					close(*tmpsockfd);
					break;
				}

				rxHandler(msg,*tmpsockfd, ret);
			}
			close(*tmpsockfd);
			delete tmpsockfd;
			free(msg);
			return;
		};
	}
}

int UhconnTCPServer::Send(const char * buf, int len, int sockFd)
{
	// return send(sockFd,buf,len,0);
	static std::mutex mlock;
	if ( sockFd < 0 ) {
        cout << "Send failed as socket not created!!" << endl;
        return err_fd;
    }

	int total_bytes = 0;
	const char* pdata = buf;
	int slen = len;
    std::lock_guard<std::mutex> guard(mlock);
    while(total_bytes < slen) {

        int ret = send(sockFd, (unsigned char*)pdata + total_bytes, slen-total_bytes, 0);
        if(ret < 0) {
			std::cout << "Send failed: " << strerror(errno) << endl;
			return err_send;
        }
        else {
            total_bytes += ret;
        }
    }    
    return total_bytes;
}

void UhconnTCPServer::clean()
{

}

void UhconnTCPServer::detach()
{
	close(sockfd);
	close(newsockfd);
} 

// void UhconnTCPServer::setRxHandler(rx_handler h, void * p){
// 	handler = h;
// 	param = p;
// }