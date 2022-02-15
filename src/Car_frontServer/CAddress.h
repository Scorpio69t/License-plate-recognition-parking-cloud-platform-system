#ifndef CADDRESS_H
#define CADDRESS_H
#include <string.h>
#include <stdlib.h>
class CAddress
{
public:
	CAddress();
	~CAddress();
	void setIp(char* ip_addr);
	void setPort(unsigned short port);
	char* getIp();
	unsigned short getPort();
private:
	char ip[20];//存放IP地址
	unsigned short port;//存放端口号
};


#endif // !CADDRESS_H

