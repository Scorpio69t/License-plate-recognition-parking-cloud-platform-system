#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <pthread.h>
#include "SingalDb.h"
#include "define_file.h"
#include "UserModel.h"
#include "CarModel.h"
#include "ShmFifo.h"
#include <list>
using namespace std;

int msgid;//消息队列id
int msgid_back;//反馈消息队列id
char* addr_back;//反馈共享内存
ShmFifo shm_fifo1(11, 10, 1024);
ShmFifo shm_fifo2(22, 10, 1024);



void* pthread_fun(void* arg)
{
	//int del=CarModel::getInstance()->deletecar();
	//list<string> testlist = CarModel::getInstance()->select_allcarmsg("闽AX6700","2021-08-17 19:29:47","2021-09-17 19:35:00");
	//list<string> carlist = CarModel::getInstance()->select_carmsg("闽AX6700");
	//cout << "testlist.size=" << testlist.size() << endl;
	//cout << "carlist.size=" << carlist.size() << endl;
	//cout << "size=" << testlist.size() << endl;
	//cout << "res=" << res << endl;
	char* addr = (char*)arg;
	while (true)
	{
		//usleep(300);
		MYBUF* msg = (MYBUF*)malloc(sizeof(MYBUF));
		int ret = msgrcv(msgid, msg, sizeof(MYBUF) - sizeof(long), 1, 0);//需要减去消息队列sizeof(long)否则会段错误越界
		if (ret == -1)
		{
			perror("read");
		}

		char buffer[1024] = { 0 };
		memcpy(buffer, shm_fifo1.shm_read(), sizeof(buffer));
		PACK_HEAD* head = (PACK_HEAD*)malloc(sizeof(PACK_HEAD));
		memcpy(head, buffer, sizeof(PACK_HEAD));

		//cout << "message=" << head->bodyType << endl;

		if (head->bodyType == REGIST_TYPE)//注册
		{
			//cout << "regist" << endl;
			PACK_REGIST_LOGIN* regist_login = (PACK_REGIST_LOGIN*)malloc(sizeof(PACK_REGIST_LOGIN));
			memcpy(regist_login, buffer+sizeof(PACK_HEAD), sizeof(PACK_REGIST_LOGIN));
			int fd_temp;
			memcpy(&fd_temp, buffer + sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN), sizeof(int));
			//cout << "name=" << regist_login->name<< endl;
			//cout << "pwd=" << regist_login->name << endl;
			//cout << "fd_temp=" << fd_temp << endl;
			int number = UserModel::getInstance()->insertUser(regist_login->name,regist_login->pwd);
			//消息队列结构体
			MYBUF msg_back;
			msg_back.mtype = 2;
			PACK_RL_BACK result;
			if (number == 0)
			{
				result.result = 0;//注册成功
			}
			else if(number==-1)
			{
				result.result = 1;//注册失败
			}
			char send_buffer[sizeof(PACK_HEAD) + sizeof(PACK_RL_BACK) + sizeof(int)] = { 0 };
			memcpy(send_buffer,head,sizeof(PACK_HEAD));
			memcpy(send_buffer+sizeof(PACK_HEAD), &result, sizeof(PACK_RL_BACK));
			memcpy(send_buffer + sizeof(PACK_HEAD) + sizeof(PACK_RL_BACK), &fd_temp, sizeof(int));
			shm_fifo2.shm_write(send_buffer);
			//消息队列发送
			int res = msgsnd(msgid_back, &msg_back, sizeof(MYBUF) - sizeof(long), 0);//需要减去消息队列sizeof(long)否则会段错误越界
			
		}
		else if (head->bodyType == LOGIN_TYPE)//登录
		{
			//cout << "login" << endl;
			PACK_REGIST_LOGIN* regist_login = (PACK_REGIST_LOGIN*)malloc(sizeof(PACK_REGIST_LOGIN));
			memcpy(regist_login, buffer + sizeof(PACK_HEAD), sizeof(PACK_REGIST_LOGIN));
			int fd_temp;
			memcpy(&fd_temp, buffer + sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN), sizeof(int));
			cout << "name=" << regist_login->name << endl;
			cout << "pwd=" << regist_login->name << endl;
			cout << "fd_temp=" << fd_temp << endl;
			int number = UserModel::getInstance()->searchUser(regist_login->name, regist_login->pwd);
			//消息队列结构体
			MYBUF msg_back;
			msg_back.mtype = LOGIN_TYPE;
			
			PACK_RL_BACK result;
			if (number == 0)
			{
				result.result = CarModel::getInstance()->totalCar(); //登录成功
			}
			else
			{
				result.result = -1; //登录失败
			}
			cout << result.result << "==========" << endl;
			char send_buffer[sizeof(PACK_HEAD) + sizeof(PACK_RL_BACK) + sizeof(int)] = { 0 };
			memcpy(send_buffer, head, sizeof(PACK_HEAD));
			memcpy(send_buffer + sizeof(PACK_HEAD), &result, sizeof(PACK_RL_BACK));
			memcpy(send_buffer + sizeof(PACK_HEAD) + sizeof(PACK_RL_BACK), &fd_temp, sizeof(int));
			shm_fifo2.shm_write(send_buffer);
			//消息队列发送
			int res = msgsnd(msgid_back, &msg_back, sizeof(MYBUF) - sizeof(long), 0);//需要减去消息队列sizeof(long)否则会段错误越界
		}
		else if (head->bodyType == CAR_GETIN)//入场包
		{
			PACK_ENTER* car_enter = (PACK_ENTER*)malloc(sizeof(PACK_ENTER));
			memcpy(car_enter, buffer + sizeof(PACK_HEAD), sizeof(PACK_ENTER));
			int fd_temp;
			memcpy(&fd_temp, buffer + sizeof(PACK_HEAD) + sizeof(PACK_ENTER), sizeof(int));
			cout << "name=" << car_enter->car_num << endl;
			cout << "fd_temp=" << fd_temp << endl;
			int number = CarModel::getInstance()->insertCarMsg(car_enter->car_num, car_enter->now_time, car_enter->location,car_enter->photo_path);
			//消息队列结构体
			MYBUF msg_back;
			msg_back.mtype = 2;

			PACK_RL_BACK result;
			memset(&result, 0x0, sizeof(PACK_RL_BACK));
			if (number == 0)
			{
				result.result = 0;//成功
			}
			else
			{
				result.result = 1;//失败
			}
			char send_buffer[sizeof(PACK_HEAD) + sizeof(PACK_RL_BACK) + sizeof(int)] = { 0 };
			memcpy(send_buffer, head, sizeof(PACK_HEAD));
			memcpy(send_buffer + sizeof(PACK_HEAD), &result, sizeof(PACK_RL_BACK));
			memcpy(send_buffer + sizeof(PACK_HEAD) + sizeof(PACK_RL_BACK), &fd_temp, sizeof(int));
			shm_fifo2.shm_write(send_buffer);
			//消息队列发送
			int res = msgsnd(msgid_back, &msg_back, sizeof(MYBUF) - sizeof(long), 0);//需要减去消息队列sizeof(long)否则会段错误越界
		}
		else if (head->bodyType == CAR_GETOUT)//出场
		{
			//cout << "car out" << endl;
			PACK_EXIT* car_exit = (PACK_EXIT*)malloc(sizeof(PACK_EXIT));
			memcpy(car_exit, buffer + sizeof(PACK_HEAD), sizeof(PACK_EXIT));
			int fd_temp;
			memcpy(&fd_temp, buffer + sizeof(PACK_HEAD) + sizeof(PACK_EXIT), sizeof(int));
			//cout << "name=" << car_exit->car_number << endl;
			//cout << "fd_temp=" << fd_temp << endl;
			
			//消息队列结构体
			MYBUF msg_back;
			msg_back.mtype = 2;
			list<string> listtemp;
			listtemp = CarModel::getInstance()->select_intime(car_exit->car_number);
			cout <<"size="<< listtemp.size() << endl;
			PACK_EXIT_BACK car_exit_back;
			memset(&car_exit_back, 0x0, sizeof(PACK_EXIT_BACK));
			if (listtemp.size() == 0)
			{
				car_exit_back.vip = -1;
			}
			else
			{
				string in_time=listtemp.front();
				int vip = stoi(listtemp.back());
				int in_year, in_month, in_day, in_hour, in_min, in_sec;//入场
				int out_year, out_month, out_day, out_hour, out_min, out_sec;//出场
				sscanf(in_time.c_str(), "%d-%2d-%2d %2d:%2d:%2d", &in_year, &in_month, &in_day, &in_hour, &in_min, &in_sec);
				sscanf(car_exit->out_time, "%d-%2d-%2d %2d:%2d:%2d", &out_year, &out_month, &out_day, &out_hour, &out_min, &out_sec);
				int year, month, day, hour, min, sec;//停车时长
				year = out_year - in_year;
				month = out_month - in_month;
				day = out_day - in_day;
				hour = out_hour - in_hour;
				if (hour <= 0)
				{
					hour *= (-1);
				}	
				min = out_min - in_min;
				if (min < 0)
				{
					min *= (-1);
				}
				sec = out_sec - in_sec;
				if (sec < 0)
				{
					sec *= (-1);
				}	
				char stay_time[64] = { 0 };
				sprintf(stay_time,"%d-%d-%d %d:%d:%d",year,month,day,hour,min,sec);
				cout << stay_time << endl;
				if (vip == 1)
				{
					car_exit_back.money = 0;
				}
				else
				{
					if (hour == 0)
					{
						car_exit_back.money = 30;
					}
					else
					{
						car_exit_back.money = hour * 30;
					}
				}
				strcpy(car_exit_back.total_time, stay_time);
				strcpy(car_exit_back.in_time, listtemp.front().c_str());
				int updatemsg = CarModel::getInstance()->updateCarmsg(car_exit->car_number, car_exit->out_time,car_exit_back.money);
			}
			char send_buffer[sizeof(PACK_HEAD) + sizeof(PACK_EXIT_BACK) + sizeof(int)] = { 0 };
			memcpy(send_buffer, head, sizeof(PACK_HEAD));
			memcpy(send_buffer + sizeof(PACK_HEAD), &car_exit_back, sizeof(PACK_EXIT_BACK));
			memcpy(send_buffer + sizeof(PACK_HEAD) + sizeof(PACK_EXIT_BACK), &fd_temp, sizeof(int));
			shm_fifo2.shm_write(send_buffer);
			//往消息队列发消息
			int res = msgsnd(msgid_back, &msg_back, sizeof(MYBUF) - sizeof(long), 0);//需要减去消息队列sizeof(long)否则会段错误越界
		}
		else if (head->bodyType == CAR_MSG_TYPE)//车辆信息包
		{
			//cout << "car_msg" << endl;
			PACK_CARMSG* car_msg = (PACK_CARMSG*)malloc(sizeof(PACK_CARMSG));
			memcpy(car_msg, buffer + sizeof(PACK_HEAD), sizeof(PACK_CARMSG));
			int fd_temp;
			memcpy(&fd_temp, buffer + sizeof(PACK_HEAD) + sizeof(PACK_CARMSG), sizeof(int));
			//cout << "name=" << car_msg->car_number << endl;
			//cout << "fd_temp=" << fd_temp << endl;
			list<string> car_msglist;
			car_msglist.clear();
			if (car_msg->car_number != "" && car_msg->in_time!= "" &&car_msg->out_time!="")
			{
				//cout << "here" << endl;
				car_msglist = CarModel::getInstance()->select_allcarmsg(car_msg->car_number, car_msg->in_time, car_msg->out_time, car_msg->page);
			}
			else if(car_msg->car_number == "" && car_msg->in_time != "" && car_msg->out_time != "")
			{
				car_msglist = CarModel::getInstance()->select_carmsg_time(car_msg->in_time, car_msg->out_time, car_msg->page);
			}
			else if (car_msg->car_number != "" && car_msg->in_time == "" && car_msg->out_time == "")
			{
				car_msglist = CarModel::getInstance()->select_carmsg(car_msg->car_number, car_msg->page);
			}
			else if (car_msg->car_number == "" && car_msg->in_time != "" && car_msg->out_time == "")
			{
				car_msglist = CarModel::getInstance()->select_carmsg_intime(car_msg->in_time, car_msg->page);
			}
			else if (car_msg->car_number == "" && car_msg->in_time == "" && car_msg->out_time != "")
			{
				car_msglist = CarModel::getInstance()->select_carmsg_outtime(car_msg->out_time, car_msg->page);
			}
			else if (car_msg->car_number == "" && car_msg->in_time == "" && car_msg->out_time == "")
			{
				//cout << "做一个空返回回去" << endl;
			}
			//cout << "list.size=" << car_msglist.size() << endl;
			list<string>::iterator car_it;
			int i = 0;
			PACK_ALLCARMSG_BACK carmsg_allback;
			memset(&carmsg_allback, 0x0, sizeof(PACK_ALLCARMSG_BACK));

			for (car_it = car_msglist.begin(); car_it != car_msglist.end(); )
			{
				strcpy(carmsg_allback.arr[i].car_number, (*car_it++).c_str());
				strcpy(carmsg_allback.arr[i].in_time, (*car_it++).c_str());
				strcpy(carmsg_allback.arr[i].out_time, (*car_it++).c_str());
				carmsg_allback.arr[i].money = stoi(*car_it++);
				cout << carmsg_allback.arr[i].car_number << "," << carmsg_allback.arr[i].in_time << "," << carmsg_allback.arr[i].out_time << "," << carmsg_allback.arr[i].money << "," << endl;
				i++;
			}
			carmsg_allback.realCount = car_msglist.size() / 4;
			//int number = CarModel::getInstance()->insertCarMsg(car_msg->car_num, car_enter->now_time, car_enter->photo_path, car_enter->location);
			//消息队列结构体
			MYBUF msg_back;
			msg_back.mtype = 2;

			char send_buffer[sizeof(PACK_HEAD) + sizeof(PACK_ALLCARMSG_BACK) + sizeof(int)] = { 0 };
			memcpy(send_buffer, head, sizeof(PACK_HEAD));
			memcpy(send_buffer + sizeof(PACK_HEAD), &carmsg_allback, sizeof(PACK_ALLCARMSG_BACK));
			memcpy(send_buffer + sizeof(PACK_HEAD) + sizeof(PACK_ALLCARMSG_BACK), &fd_temp, sizeof(int));
			shm_fifo2.shm_write(send_buffer);
			//消息队列发送
			int res = msgsnd(msgid_back, &msg_back, sizeof(MYBUF) - sizeof(long), 0);//需要减去消息队列sizeof(long)否则会段错误越界
		}
		else if (head->bodyType == VIDEO_TYPE)//视频信息包
		{
			//cout << "video" << endl;
			PACK_VIDEO* video = (PACK_VIDEO*)malloc(sizeof(PACK_VIDEO));
			memcpy(video, buffer + sizeof(PACK_HEAD), sizeof(PACK_VIDEO));
			int fd_temp;
			memcpy(&fd_temp, buffer + sizeof(PACK_HEAD) + sizeof(PACK_VIDEO), sizeof(int));
			//cout << "name=" << video->video_name << endl;
			//cout << "fd_temp=" << fd_temp << endl;
			int number = CarModel::getInstance()->insertNewVideo(video->video_name, video->totalFrameCount);
			if (number == 0)
			{
				//cout << "success" << endl;
			}
		}
		else if (head->bodyType == VIDEO_OPEN)//视频打开包
		{
			//cout << "video_open" << endl;
			PACK_VIDEO* video = (PACK_VIDEO*)malloc(sizeof(PACK_VIDEO));
			memcpy(video, buffer + sizeof(PACK_HEAD), sizeof(PACK_VIDEO));
			int fd_temp;
			memcpy(&fd_temp, buffer + sizeof(PACK_HEAD) + sizeof(PACK_VIDEO), sizeof(int));
			//cout << "name=" << video->video_name << endl;
			//cout << "fd_temp=" << fd_temp << endl;
			int frame = CarModel::getInstance()->selectVideoFrame(video->video_name);
			PACK_VIDEO_BACK video_back;
			video_back.frameCount = frame;
			//消息队列结构体
			MYBUF msg_back;
			msg_back.mtype = 2;

			char send_buffer[sizeof(PACK_HEAD) + sizeof(PACK_VIDEO_BACK) + sizeof(int)] = { 0 };
			memcpy(send_buffer, head, sizeof(PACK_HEAD));
			memcpy(send_buffer + sizeof(PACK_HEAD), &video_back, sizeof(PACK_VIDEO_BACK));
			memcpy(send_buffer + sizeof(PACK_HEAD) + sizeof(PACK_VIDEO_BACK), &fd_temp, sizeof(int));
			shm_fifo2.shm_write(send_buffer);
			//消息队列发送
			int res = msgsnd(msgid_back, &msg_back, sizeof(MYBUF) - sizeof(long), 0);//需要减去消息队列sizeof(long)否则会段错误越界
		}
		else if (head->bodyType == VIDEO_CLOSE)//视频关闭包
		{
			//cout << "video_close" << endl;
			PACK_VIDEO* video_close = (PACK_VIDEO*)malloc(sizeof(PACK_VIDEO));
			memcpy(video_close, buffer + sizeof(PACK_HEAD), sizeof(PACK_VIDEO));
			int fd_temp;
			memcpy(&fd_temp, buffer + sizeof(PACK_HEAD) + sizeof(PACK_VIDEO), sizeof(int));
			//cout << "name=" << video_close->frameCount << endl;
			//cout << "fd_temp=" << fd_temp << endl;
			int number = CarModel::getInstance()->updateVideoFrame(video_close->video_name, video_close->totalFrameCount);
			if (number == 0)
			{
				//cout << "success" << endl;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	//创建一个接受消息队列
	msgid = msgget(1234, IPC_CREAT | IPC_EXCL | 0666);
	if (msgid == -1)//创建失败 消息队列已经存在
	{
		msgid = msgget(1234, 0);//打开一个消息队列
	}
	//创建一个反馈消息队列
	msgid_back = msgget(12345, IPC_CREAT | IPC_EXCL | 0666);
	if (msgid_back == -1)//创建失败 消息队列已经存在
	{
		msgid_back = msgget(12345, 0);//打开一个消息队列
	}
	//开启一个线程
	cout << "BackServer  open!!!!" << endl;
	pthread_t pthread_id;
	pthread_create(&pthread_id, NULL, pthread_fun, NULL);//开启一个线程
	while (true)
	{
		
	}
	return 0;
}