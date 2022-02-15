#ifndef  TCPSOCKETSERVER_H
#define  TCPSOCKETSERVER_H

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/epoll.h>
using namespace std;
#include "CSocketBase.h"
#include "CAddress.h"

#define SERVER_PORT 8090

class TCPSocketServer :public CSocketBase
{
public:
	TCPSocketServer();
	~TCPSocketServer();
	void Start();
	void Run();
	void Stop();
private:
	struct sockaddr_in s_addr;
	CAddress* my_ip_port;
};
#endif // ! TCPSOCKETSERVER_H
