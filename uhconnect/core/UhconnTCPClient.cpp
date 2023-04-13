#include "UhconnTCPClient.h"
#include <time.h>
#include <errno.h>

UhconnTCPClient::UhconnTCPClient()
{
    sock = -1;
	port = 0;
	address = "";
}

UhconnTCPClient::~UhconnTCPClient()
{
    if(sock>0){
		close(sock);
	}
}

bool UhconnTCPClient::setup(string address , int port)
{
	#ifdef DEBUGINFO
	cout << "client connect to: addr->" << address << " port->" << port << endl;
	#endif
	
  	if(sock == -1)
	{
		sock = socket(AF_INET , SOCK_STREAM , 0);
		if (sock == -1)
		{
      		cout << "Could not create socket" << endl;
    	}
    }
  	if(inet_addr(address.c_str()) == 0) //@todo
  	{
		struct hostent *he;
		struct in_addr **addr_list;
		if ((he = gethostbyname(address.c_str())) == NULL)
		{
			herror("gethostbyname");
				cout<<"Failed to resolve hostname\n";
			return false;
		}
	   	addr_list = (struct in_addr **) he->h_addr_list;
		for(int i = 0; addr_list[i] != NULL; i++)
		{
			server.sin_addr = *addr_list[i];
			break;
		}
  	}
  	else
  	{
    	server.sin_addr.s_addr = inet_addr(address.c_str());
  	}
  	server.sin_family = AF_INET;
  	server.sin_port = htons( port );
  	if (connect(sock, (struct sockaddr *)&server , sizeof(server)) < 0)
  	{
		perror("connect failed. Error");
		return false;
  	}

	// struct timeval timeout={1,500*1000};//
    // setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
    // setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
  	return true;
}

// bool UhconnTCPClient::Send(string data)
// {
// 	int ret = 0;
// 	if(sock != -1) 
// 	{
// 		ret = send(sock , data.c_str() , strlen( data.c_str() ) , 0);
// 		//std::cout << "UhconnTCPClient::Send() ret "<< ret <<std::endl;
// 		if( ret < 0)
// 		{
// 			cout << "Send failed : " << data << endl;
// 			return false;
// 		}
// 	}
// 	else{
// 		cout << "Send failed as socket not created!!" << endl;
//         return false;
// 	}
		
// 	return true;
// }
bool UhconnTCPClient::Send(const string& data) {
    if (sock == -1) {
        cerr << "Send failed as socket not created!!" << endl;
        return false;
    }
    
    // int ret = send(sock, data.c_str(), data.length(), 0);
    // if (ret == -1) {
    //     cerr << "Send failed: " << strerror(errno) << endl;
    //     return false;
    // }
	int total_bytes = 0;
	const char* pdata = data.c_str();
	int slen = data.length();
    while(total_bytes < slen) {
        //ret = router->tcpDataServer.Send((unsigned char*)block->data + total_bytes, block->len-total_bytes, fd);
        int ret = send(sock, (unsigned char*)pdata + total_bytes, slen-total_bytes, 0);
        if(ret < 0) {
			cerr << "Send failed: " << strerror(errno) << endl;
			return false;
        }
        else {
            total_bytes += ret;
        }
    }    
    return true;
}
// string UhconnTCPClient::receive(int size)
// {
//   	char buffer[size];
// 	memset(&buffer[0], 0, sizeof(buffer));
//     int ret;
//   	string reply;
// 	ret = recv(sock , buffer , size, 0);
// 	if( ret < 0)
//   	{
// 	    cout << "tcp client string receive failed! ret=" << ret <<
// 		"errno="<<errno<<strerror(errno)<<endl;
// 		return nullptr;
//   	}
// 	buffer[size-1]='\0';
//   	reply = buffer;
//   	return reply;
// }

int UhconnTCPClient::receive(unsigned char * buf, int size)
{
	int ret;
    ret = recv(sock , buf, size, 0);
	if(ret < 0)
  	{
	    cout << "tcp client buff receive failed! err=" << ret <<
		"errno="<<errno<<" "<<strerror(errno)<<endl;
  	}
  	return ret;
}

string UhconnTCPClient::read()
{
  	char buffer[1] = {};
  	string reply;
  	while (buffer[0] != '\n') {
    		if( recv(sock , buffer , sizeof(buffer) , 0) < 0)
    		{
      			cout << "tcp client read failed!" << endl;
			return nullptr;
    		}
		reply += buffer[0];
	}
	return reply;
}

void UhconnTCPClient::exit()
{
    close(sock);
	sock = -1;
}
