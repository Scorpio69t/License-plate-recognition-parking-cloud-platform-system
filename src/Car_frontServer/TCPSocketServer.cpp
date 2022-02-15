#include "TCPSocketServer.h"
#define ERR_EXIT(m)\
	do{\
		perror(m);\
		_exit(EXIT_FAILURE);\
	}while (0);

void TCPSocketServer::Start()
{
	this->socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->socket_fd < 0)
	{
		ERR_EXIT("socket_fd:");
	}
	this->Run();
}

void TCPSocketServer::Run()
{
	//做绑定和监听实现
	//IP地址也有 v4 v6区分，我们要把地址封装成类
	this->my_ip_port = new CAddress();
	this->my_ip_port->setPort(SERVER_PORT);//8090
	this->my_ip_port->setIp("192.168.142.136");
	//填充addr这个结构体
	this->s_addr.sin_family = AF_INET;//协议家族  ipv4
	this->s_addr.sin_port = htons(this->my_ip_port->getPort());//端口（网络字节序   主机字节序）
	this->s_addr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY本机的任意ip 如果你的电脑ip是 192.168.52.128 那么服务器的ip就是192.168.52.128
	//this->s_addr.sin_addr.s_addr =inet_addr("116.62.144.231");
	//inet_aton(this->my_ip_port->getIp(), &s_addr.sin_addr);

	//允许端口重复使用，端口立即使用，不用等待
	int on = 1;
	int ret = setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (ret < 0)
	{
		ERR_EXIT("setsockopt");
	}
		
	//二：绑定ip端口
	ret = bind(this->socket_fd, (struct sockaddr*)&this->s_addr, sizeof(struct sockaddr_in));
	if (ret < 0)
	{
		ERR_EXIT("bind");
	}
		
	//三：监听
	ret = listen(this->socket_fd, SOMAXCONN);
	if (ret < 0)
	{
		ERR_EXIT("listen");
	}
		
}
void TCPSocketServer::Stop()
{

}

TCPSocketServer::TCPSocketServer()
{

}

TCPSocketServer::~TCPSocketServer()
{

}