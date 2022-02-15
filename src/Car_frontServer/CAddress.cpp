#include "CAddress.h"

CAddress::CAddress()
{
}

CAddress::~CAddress()
{
}
void CAddress::setIp(char* ip_addr)
{
	memset(this->ip, 0x0, sizeof(this->ip));
	memcpy(this->ip, ip_addr, strlen(ip_addr));
}
void CAddress::setPort(unsigned short port)
{
	this->port = port;
}
char* CAddress::getIp()
{
	return this->ip;
}
unsigned short CAddress::getPort()
{
	return this->port;
}
