#include "EPollServer.h"
#include <stdlib.h>
#include <stdio.h>
//描述性错误消息输出到标准错误
#define ERR_EXIT(m)\
	do{\
		perror(m);\
		_exit(EXIT_FAILURE);\
	}while (0);

int msgid_back;//反馈消息队列id
ShmFifo shm_fifo2(22, 10, 1024);
//线程运行函数
void* pthread_fun(void* arg)
{
	while (1)
	{
		MYBUF* msg = (MYBUF*)malloc(sizeof(MYBUF));
		//需要减去消息队列sizeof(long)否则会段错误越界
		int ret = msgrcv(msgid_back, msg, sizeof(MYBUF) - sizeof(long), 2, 0);
		if (ret == -1)
		{
			perror("msgrcv");
		}
		//从共享内存读数据
		char rec_buffer[1024] = { 0 };
		memcpy(rec_buffer, shm_fifo2.shm_read(), sizeof(rec_buffer));

		PACK_HEAD* head = (PACK_HEAD*)malloc(sizeof(PACK_HEAD));
		memcpy(head, rec_buffer, sizeof(PACK_HEAD));
		int fd_temp = 0;
		//日志打印
		PrintLog::getInstance()->send_packCounts++;
		PrintLog::getInstance()->print_log();
		if (head->bodyType == REGIST_TYPE)//注册
		{

			//定义注册包体
			char body_buffer[sizeof(PACK_RL_BACK)] = { 0 };
			memcpy(body_buffer, rec_buffer + sizeof(PACK_HEAD), sizeof(PACK_RL_BACK));
			memcpy(&fd_temp, rec_buffer + sizeof(PACK_HEAD)+sizeof(PACK_RL_BACK), sizeof(int));
			char total_buffer[sizeof(PACK_HEAD) + sizeof(PACK_RL_BACK)] = { 0 };
			memcpy(total_buffer, head, sizeof(PACK_HEAD));
			memcpy(total_buffer+ sizeof(PACK_HEAD), body_buffer, sizeof(body_buffer));
			int res = write(fd_temp, total_buffer, sizeof(total_buffer));
		}
		else if (head->bodyType == LOGIN_TYPE)//登录
		{
			char body_buffer[sizeof(PACK_RL_BACK)] = { 0 };
			memcpy(body_buffer, rec_buffer + sizeof(PACK_HEAD), sizeof(PACK_RL_BACK));
			memcpy(&fd_temp, rec_buffer + sizeof(PACK_HEAD) + sizeof(PACK_RL_BACK), sizeof(int));
			char total_buffer[sizeof(PACK_HEAD) + sizeof(PACK_RL_BACK)] = { 0 };
			memcpy(total_buffer, head, sizeof(PACK_HEAD));
			memcpy(total_buffer + sizeof(PACK_HEAD), body_buffer, sizeof(body_buffer));
			int res = write(fd_temp, total_buffer, sizeof(total_buffer));
		}
		else if (head->bodyType == CAR_GETIN)//入场
		{
			char body_buffer[sizeof(PACK_RL_BACK)] = { 0 };
			memcpy(body_buffer, rec_buffer + sizeof(PACK_HEAD), sizeof(PACK_RL_BACK));
			memcpy(&fd_temp, rec_buffer + sizeof(PACK_HEAD) + sizeof(PACK_RL_BACK), sizeof(int));
			char total_buffer[sizeof(PACK_HEAD) + sizeof(PACK_RL_BACK)] = { 0 };
			memcpy(total_buffer, head, sizeof(PACK_HEAD));
			memcpy(total_buffer + sizeof(PACK_HEAD), body_buffer, sizeof(body_buffer));
			int res = write(fd_temp, total_buffer, sizeof(total_buffer));
		}
		else if (head->bodyType == CAR_GETOUT)//出场
		{
			char body_buffer[sizeof(PACK_EXIT_BACK)] = { 0 };
			memcpy(body_buffer, rec_buffer + sizeof(PACK_HEAD), sizeof(PACK_EXIT_BACK));
			memcpy(&fd_temp, rec_buffer + sizeof(PACK_HEAD) + sizeof(PACK_EXIT_BACK), sizeof(int));
			char total_buffer[sizeof(PACK_HEAD) + sizeof(PACK_EXIT_BACK)] = { 0 };
			memcpy(total_buffer, head, sizeof(PACK_HEAD));
			memcpy(total_buffer + sizeof(PACK_HEAD), body_buffer, sizeof(body_buffer));
			int res = write(fd_temp, total_buffer, sizeof(total_buffer));
		}
		else if (head->bodyType == CAR_MSG_TYPE)//车辆信息包
		{
			char body_buffer[sizeof(PACK_ALLCARMSG_BACK)] = { 0 };
			memcpy(body_buffer, rec_buffer + sizeof(PACK_HEAD), sizeof(PACK_ALLCARMSG_BACK));
			memcpy(&fd_temp, rec_buffer + sizeof(PACK_HEAD) + sizeof(PACK_ALLCARMSG_BACK), sizeof(int));
			char total_buffer[sizeof(PACK_HEAD) + sizeof(PACK_ALLCARMSG_BACK)] = { 0 };
			memcpy(total_buffer, head, sizeof(PACK_HEAD));
			memcpy(total_buffer + sizeof(PACK_HEAD), body_buffer, sizeof(body_buffer));
			int res = write(fd_temp, total_buffer, sizeof(total_buffer));
		}
		else if (head->bodyType == VIDEO_OPEN)//视频打开包
		{
			char body_buffer[sizeof(PACK_VIDEO_BACK)] = { 0 };
			memcpy(body_buffer, rec_buffer + sizeof(PACK_HEAD), sizeof(PACK_VIDEO_BACK));
			memcpy(&fd_temp, rec_buffer + sizeof(PACK_HEAD) + sizeof(PACK_VIDEO_BACK), sizeof(int));
			char total_buffer[sizeof(PACK_HEAD) + sizeof(PACK_VIDEO_BACK)] = { 0 };
			memcpy(total_buffer, head, sizeof(PACK_HEAD));
			memcpy(total_buffer + sizeof(PACK_HEAD), body_buffer, sizeof(body_buffer));
			int res = write(fd_temp, total_buffer, sizeof(total_buffer));

		}
		//cout << "fd_temp=" << fd_temp << endl;
		free(msg); //msg所指的内存被释放，但是msg所指的地址仍然不变
		msg = nullptr;
		free(head);
		head = nullptr;
	}
}

int main()
{
	//创建一个反馈消息队列--告诉程序该去共享内存取数据了
	msgid_back = msgget(12345, IPC_CREAT | IPC_EXCL | 0666);
	if (msgid_back == -1) //创建失败 消息队列已经存在
	{
		msgid_back = msgget(12345, 0);//打开一个消息队列
	}
	/*
	//获取当前系统时间
	time_t nowTime;
	nowTime = time(NULL);
	struct tm* sysTime = localtime(&nowTime);
	char nowtime_buf[50] = { 0 };
	sprintf(nowtime_buf, "../../../%d-%d-%d.txt", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday);

	FILE* fp = fopen(nowtime_buf,"wb+");
	*/

	//编译时注意加上-lpthread参数，以调用静态链接库。因为pthread并非Linux系统的默认库
	pthread_t pthread_id; //创建线程
	pthread_create(&pthread_id, NULL, pthread_fun, NULL); //开启一个线程

	EPollServer* epoll_server = new EPollServer();
	epoll_server->Start();
	return 0;
}
