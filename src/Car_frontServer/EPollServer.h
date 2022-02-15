#ifndef EPOLLSERVER_H
#define EPOLLSERVER_H
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
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/shm.h>
using namespace std;
#include "TCPSocketServer.h"
#include "ThreadPool.h"
#include "define_file.h"
#include "ShmFifo.h"
#include "PrintLog.h"
#include <time.h>
#include <map>
class MyTask :public TaskBase
{
public:
	void Run();
	MyTask(int fd,int msgid);
private:
	char* data;
	int fd;
	int msgid;
};

class EPollServer
{
public:
	EPollServer();
	~EPollServer();
	void Start();
	void Run();
	void Stop();
	bool Epoll_Add(int fd, int event_type);
	bool Epoll_Del(int fd, int event_type);
	void epoll_recv(int fd,int msgid);
	static map<int, PACK_HEART> socket_map;
	static pthread_mutex_t mutex;
private:
	int epoll_fd;//epoll文件描述符
	TCPSocketServer* server_socket;
	int epoll_wait_return;
	struct epoll_event event;
	struct epoll_event epoll_events[20];
	ThreadPool* pool;//定义一个线程池对象
	int msgid;//接收消息队列id
	
	pthread_t pthread_id;
	
};
#endif // !EPOLLSERVER_H

