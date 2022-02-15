#include "EPollServer.h"
//创建一个共享内存
ShmFifo shm_fifo1(11 ,10, 1024);
map<int, PACK_HEART> EPollServer::socket_map;
pthread_mutex_t EPollServer::mutex;
#define ERR_EXIT(m)\
	do{\
		perror(m);\
		_exit(EXIT_FAILURE);\
	}while (0);

MyTask::MyTask(int fd, int msgid)
{
	this->fd = fd;
	this->msgid = msgid;
}
void* DoHeart(void* arg)
{
	EPollServer* temp = (EPollServer*)arg;
	while (true)
	{
		sleep(1);
		map<int,PACK_HEART>::iterator it;
		for (it = EPollServer::socket_map.begin(); it != EPollServer::socket_map.end(); it++)
		{
			(*it).second.time--;
			//cout << "time=" << (*it).second.time << endl;
			if ((*it).second.time == 0)
			{
				//cout << "closehere" << endl;
				pthread_mutex_lock(&EPollServer::mutex);//锁住
				close((*it).second.fd);
				EPollServer::socket_map.erase(it);
				pthread_mutex_unlock(&EPollServer::mutex);//锁住
			}
		}
	}
}
void MyTask::Run()
{
	//获取当前系统时间
	time_t nowTime;
	nowTime = time(NULL);
	struct tm* sysTime = localtime(&nowTime);
	char nowtime_buf[50] = { 0 };
	char nowtime_buffer[50] = { 0 };
	sprintf(nowtime_buf, "./log/%d-%02d-%02d.txt", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday);
	sprintf(nowtime_buffer, "%d-%02d-%02d/%02d:%02d:%02d", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday,
														   sysTime->tm_hour, sysTime->tm_min, sysTime->tm_sec);

	//有客户端发送数据过来
	MYBUF msg;
	msg.mtype = 1;
	PACK_HEAD head;
	bzero(&head, sizeof(PACK_HEAD));
	int ret = read(this->fd, &head, sizeof(head));
	//进行分批操作
	if (ret > 0)
	{
		PrintLog::getInstance()->rec_packCounts++;
		if (head.bodyType == REGIST_TYPE)//注册
		{

			PACK_REGIST_LOGIN regist_login;
			int res = read(this->fd, &regist_login, sizeof(PACK_REGIST_LOGIN));
			PrintLog::getInstance()->write_login_log(nowtime_buffer, "注册", 100, "123", regist_login.name, regist_login.pwd);

			char buffer[sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN) + sizeof(int)] = { 0 };
			//拷贝包头
			memcpy(buffer, &head, sizeof(PACK_HEAD));
			//拷贝包体
			memcpy(buffer + sizeof(PACK_HEAD), &regist_login, sizeof(PACK_REGIST_LOGIN));
			//拷贝fd
			memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN), &this->fd, sizeof(int));
			if (res > 0)
			{
				shm_fifo1.shm_write(buffer);
				//往消息队列发消息
				int res = msgsnd(msgid, &msg, sizeof(MYBUF) - sizeof(long), 0);//需要减去消息队列sizeof(long)否则会段错误越界
			}
		}
		else if (head.bodyType == LOGIN_TYPE)//登录
		{
			PrintLog::getInstance()->userCounts++;
			PACK_REGIST_LOGIN regist_login;
			int res = read(this->fd, &regist_login, sizeof(PACK_REGIST_LOGIN));
			PrintLog::getInstance()->write_login_log(nowtime_buffer,"登录",100,"123",regist_login.name,regist_login.pwd);

			char buffer[sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN) + sizeof(int)] = { 0 };
			//拷贝包头
			memcpy(buffer, &head, sizeof(PACK_HEAD));
			//拷贝包体
			memcpy(buffer + sizeof(PACK_HEAD), &regist_login, sizeof(PACK_REGIST_LOGIN));
			//拷贝fd
			memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN), &this->fd, sizeof(int));
			if (res > 0)
			{
				cout << "msg=" << head.bodyType << endl;
				shm_fifo1.shm_write(buffer);
				int res = msgsnd(msgid, &msg, sizeof(MYBUF) - sizeof(long), 0);//需要减去消息队列sizeof(long)否则会段错误越界
			}
		}
		else if (head.bodyType == CAR_GETIN)//入场包
		{
			PACK_ENTER car_enter;
			int res = read(this->fd, &car_enter, sizeof(PACK_ENTER));
			PrintLog::getInstance()->write_carin_log(nowtime_buffer, "入场", 100, "123", car_enter.car_num,this->fd,car_enter.now_time);
			cout << "car1=" << car_enter.number << endl;
			cout << "car2=" << car_enter.car_num << endl;
			cout << "car3=" << car_enter.location << endl;
			cout << "car4=" << car_enter.now_time << endl;
			cout << "car5=" << car_enter.photo_path << endl;
			char buffer[sizeof(PACK_HEAD) + sizeof(PACK_ENTER) + sizeof(int)] = { 0 };
			//拷贝包头
			memcpy(buffer, &head, sizeof(PACK_HEAD));
			//拷贝包体
			memcpy(buffer + sizeof(PACK_HEAD), &car_enter, sizeof(PACK_ENTER));
			//拷贝fd
			memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_ENTER), &this->fd, sizeof(int));
			if (res > 0)
			{
				shm_fifo1.shm_write(buffer);
				int res = msgsnd(msgid, &msg, sizeof(MYBUF) - sizeof(long), 0);//需要减去消息队列sizeof(long)否则会段错误越界
			}

		}
		else if (head.bodyType == CAR_GETOUT)//出场包
		{
			PACK_EXIT car_exit;
			int res = read(this->fd, &car_exit, sizeof(PACK_EXIT));
			PrintLog::getInstance()->write_carout_log(nowtime_buffer, "出场", 100, "123", car_exit.car_number, this->fd, car_exit.out_time);
			//cout << "car1=" << car_exit.car_number << endl;
			//cout << "car2=" << car_exit.out_time << endl;
			char buffer[sizeof(PACK_HEAD) + sizeof(PACK_EXIT) + sizeof(int)] = { 0 };
			//拷贝包头
			memcpy(buffer, &head, sizeof(PACK_HEAD));
			//拷贝包体
			memcpy(buffer + sizeof(PACK_HEAD), &car_exit, sizeof(PACK_EXIT));
			//拷贝fd
			memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_EXIT), &this->fd, sizeof(int));
			if (res > 0)
			{
				shm_fifo1.shm_write(buffer);
				int res = msgsnd(msgid, &msg, sizeof(MYBUF) - sizeof(long), 0);//需要减去消息队列sizeof(long)否则会段错误越界
			}

		}
		else if (head.bodyType == HEART_TYPE)//心跳包
		{
			map<int,PACK_HEART>::iterator it=EPollServer::socket_map.find(this->fd);
			if (it!= EPollServer::socket_map.end())
			{
				pthread_mutex_lock(&EPollServer::mutex);//锁住
				(*it).second.time = 128;
				cout << "hearthere" << endl;
				pthread_mutex_unlock(&EPollServer::mutex);//解锁
			}
		}
		else if (head.bodyType == PHOTO_TYPE)//图片包
		{
			PACK_PHOTO photo_pack;
			PACK_PHOTO_BACK photo_result;
			int res = read(this->fd, &photo_pack, sizeof(PACK_PHOTO));
			char fileName[24] = { 0 };
			sprintf(fileName, "./photo/%s", photo_pack.filename);
			cout << photo_pack.filename << endl;
			FILE* fp;                //定义一个文件指针
			fp = fopen(fileName,"a+");   //以读写方式打开或建立一个二进制文件，允许读和写。
			fwrite(photo_pack.context, photo_pack.realSize,1, fp);
			fclose(fp);
			photo_result.result = 1;
			char sendbuffer[sizeof(PACK_HEAD) + sizeof(PACK_PHOTO_BACK)] = { 0 };
			memcpy(sendbuffer, &head, sizeof(PACK_HEAD));
			memcpy(sendbuffer + sizeof(PACK_HEAD), &photo_result, sizeof(PACK_PHOTO_BACK));
			write(this->fd, sendbuffer, sizeof(sendbuffer));
		}
		else if (head.bodyType == CAR_MSG_TYPE)//车辆信息包
		{
			PACK_CARMSG car_msg;
			int res = read(this->fd, &car_msg, sizeof(PACK_CARMSG));
			PrintLog::getInstance()->write_carmsg_log(nowtime_buffer,"查询",100,"123", car_msg.car_number,car_msg.in_time,car_msg.out_time);
			//cout << "car1=" << car_msg.car_number << endl;
			//cout << "car2=" << car_msg.page << endl;
			char buffer[sizeof(PACK_HEAD) + sizeof(PACK_CARMSG) + sizeof(int)] = { 0 };
			//拷贝包头
			memcpy(buffer, &head, sizeof(PACK_HEAD));
			//拷贝包体
			memcpy(buffer + sizeof(PACK_HEAD), &car_msg, sizeof(PACK_CARMSG));
			//拷贝fd
			memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_CARMSG), &this->fd, sizeof(int));
			if (res > 0)
			{
				shm_fifo1.shm_write(buffer);
				int res = msgsnd(msgid, &msg, sizeof(MYBUF) - sizeof(long), 0);//需要减去消息队列sizeof(long)否则会段错误越界
			}
		}
		else if (head.bodyType == VIDEO_TYPE)//视频信息包
		{
			PACK_VIDEO video;
			int res = read(this->fd, &video, sizeof(PACK_VIDEO));
			PrintLog::getInstance()->write_video_log(nowtime_buffer, "添加视频", 100, "123", video.video_name, video.totalFrameCount);
			//cout << "car1=" << video.video_name << endl;
			char buffer[sizeof(PACK_HEAD) + sizeof(PACK_VIDEO) + sizeof(int)] = { 0 };
			//拷贝包头
			memcpy(buffer, &head, sizeof(PACK_HEAD));
			//拷贝包体
			memcpy(buffer + sizeof(PACK_HEAD), &video, sizeof(PACK_VIDEO));
			//拷贝fd
			memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_VIDEO), &this->fd, sizeof(int));
			if (res > 0)
			{
				shm_fifo1.shm_write(buffer);
				int res = msgsnd(msgid, &msg, sizeof(MYBUF) - sizeof(long), 0);//需要减去消息队列sizeof(long)否则会段错误越界
			}
		}
		else if (head.bodyType == VIDEO_OPEN)//视频打开包
		{
			PrintLog::getInstance()->play_counts++;

			PACK_VIDEO video;
			int res = read(this->fd, &video, sizeof(PACK_VIDEO));
			
			//cout << "car1=" << video.video_name << endl;
			char buffer[sizeof(PACK_HEAD) + sizeof(PACK_VIDEO) + sizeof(int)] = { 0 };
			//拷贝包头
			memcpy(buffer, &head, sizeof(PACK_HEAD));
			//拷贝包体
			memcpy(buffer + sizeof(PACK_HEAD), &video, sizeof(PACK_VIDEO));
			//拷贝fd
			memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_VIDEO), &this->fd, sizeof(int));
			if (res > 0)
			{
				//cout << "msg=" << head.bodyType << endl;
				shm_fifo1.shm_write(buffer);
				int res = msgsnd(msgid, &msg, sizeof(MYBUF) - sizeof(long), 0);//需要减去消息队列sizeof(long)否则会段错误越界
			}
		}
		else if (head.bodyType == VIDEO_CLOSE)//视频关闭包
		{

			PACK_VIDEO video_close;
			int res = read(this->fd, &video_close, sizeof(PACK_VIDEO));
			char buffer[sizeof(PACK_HEAD) + sizeof(PACK_VIDEO) + sizeof(int)] = { 0 };
			//拷贝包头
			memcpy(buffer, &head, sizeof(PACK_HEAD));
			//拷贝包体
			memcpy(buffer + sizeof(PACK_HEAD), &video_close, sizeof(PACK_VIDEO));
			//拷贝fd
			memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_VIDEO), &this->fd, sizeof(int));
			if (res > 0)
			{
				//cout << "msg=" << head.bodyType << endl;
				shm_fifo1.shm_write(buffer);
				int res = msgsnd(msgid, &msg, sizeof(MYBUF) - sizeof(long), 0);//需要减去消息队列sizeof(long)否则会段错误越界
			}
		}
		PrintLog::getInstance()->print_log();
	}
	else
	{
		//客户端断开连接
		cout << "client error close" << endl;
		//EPollServer::Epoll_Del(this->fd, EPOLLIN);
		close(this->fd);
		return;
	}
}
EPollServer::EPollServer()
{
	pool = new ThreadPool(20);
	//创建一个消息队列
	msgid = msgget(1234, IPC_CREAT | IPC_EXCL | 0666);
	if (msgid == -1)//创建失败 消息队列已经存在
	{
		msgid = msgget(1234, 0);//打开一个消息队列
	}
	pthread_mutex_init(&mutex, NULL);
	
}

EPollServer::~EPollServer()
{
}

void EPollServer::Start()
{
	this->epoll_fd = epoll_create(10);//创建epoll函数,在 Linux 内核 2.6.8 版本以后，这个参数是被忽略的,参数可以随便给
	if (this->epoll_fd < 0)
	{
		ERR_EXIT("epoll error:");
		exit(0);
	}
	this->Run();
}
void EPollServer::Run()
{
	//建立TCP连接
	this->server_socket = new TCPSocketServer();
	this->server_socket->Start();

	//创建心跳线程
	pthread_create(&pthread_id, NULL, DoHeart, this);
	cout << "FrontServer open!!!!" << endl;
	//把这个套接字和监听事件告诉系统
	this->Epoll_Add(this->server_socket->getSocketFd(), EPOLLIN); //把服务器这个套接字存入epoll内部

	while (true)
	{
		//当执行epoll_event函数的时候，这个函数会阻塞，直到某个套接字有数据才会解阻塞
		//第三个参数是同一时间可以允许多少个套接字，一般为前面创建的数组大小。
		//第四个参数如果没有套接字的时候一直阻塞则为-1
		//epoll_wait的返回值：如果某一时刻有多个套接字有数据，epoll_wait的返回值就是记录产生事件个数
		//调用 epoll_wait() 可以让内核去检测就绪的事件，并将就绪的事件放到就绪列表中并返回，通过返回的事件数组做进一步的事件处理
		int size = sizeof(epoll_events) / sizeof(epoll_events[0]);
		epoll_wait_return = epoll_wait(this->epoll_fd, this->epoll_events, size, -1);
		
		for (int i = 0; i < this->epoll_wait_return; i++)
		{
			//这时候判断是否是监听sockfd套接字的描述符，如果是就建立连接
			if (epoll_events[i].data.fd == this->server_socket->getSocketFd())
			{
				//客户端连接
				//这里说明服务器sock有事件，就是说明有新的客户端连接
				//accept返回 一个新的文件描述符，用来通信的（read write）  con_fd就是客户端
				//最后两个参数设置为空 表示对客户端的ip地址和端口不接受
				//accept函数第一个参数是监听的文件描述符，第二个参数是用来保存客户端套接字对应的“地方”客户端IP和端口信息等，第三个参数就是占地大小，对数据不感冒就NULL
				int con_fd = accept(this->server_socket->getSocketFd(), NULL, NULL);
				cout << "client connect!!" << endl;
				
				PACK_HEART heart;
				heart.fd = con_fd;
				heart.time = 128;
				pthread_mutex_lock(&mutex);//锁住
				
				this->Epoll_Add(con_fd, EPOLLIN); 
				socket_map.insert(pair<int, PACK_HEART>(con_fd, heart));

				pthread_mutex_unlock(&mutex);//解锁
				//日志打印
				PrintLog::getInstance()->effec_conCounts++;
				PrintLog::getInstance()->print_log();
			}
			//通讯
			else if (epoll_events[i].events && EPOLLIN)
			{
				//this->epoll_recv(epoll_events[i].data.fd, this->msgid);
				MyTask* task = new MyTask(epoll_events[i].data.fd, this->msgid);
				pool->addTask(task);
				cout << "msg!!!" << endl;
				PrintLog::getInstance()->rec_packCounts++;
				PrintLog::getInstance()->print_log();
				/*
				PrintLog::getInstance()->rec_packCounts++;
				PrintLog::getInstance()->print_log();
				//有客户端发送数据过来
				PACK_HEAD head;
				int ret = read(epoll_events[i].data.fd, &head, sizeof(head));
				//进行分批操作
				if (ret > 0)
				{
					if (head.bodyType == REGIST_TYPE)//注册
					{
						PACK_REGIST_LOGIN regist_login;
						int res = read(epoll_events[i].data.fd, &regist_login, sizeof(PACK_REGIST_LOGIN));
						char head_buffer[sizeof(PACK_HEAD)] = { 0 };
						memcpy(head_buffer, &head, sizeof(PACK_HEAD));

						char buffer[sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN)+sizeof(int)] = { 0 };
						//拷贝包头
						memcpy(buffer, &head, sizeof(PACK_HEAD));
						//拷贝包体
						memcpy(buffer + sizeof(PACK_HEAD), &regist_login, sizeof(PACK_REGIST_LOGIN));
						//拷贝fd
						memcpy(buffer + sizeof(PACK_HEAD)+sizeof(PACK_REGIST_LOGIN), &epoll_events[i].data.fd, sizeof(int));
						if (res > 0)
						{
							cout << "msg=" << head.bodyType << endl;
							shm_fifo1.shm_write(buffer);
							this->epoll_recv(epoll_events[i].data.fd,buffer);
						}
					}
					else if (head.bodyType == LOGIN_TYPE)//登录
					{
						PACK_REGIST_LOGIN regist_login;
						int res = read(epoll_events[i].data.fd, &regist_login, sizeof(PACK_REGIST_LOGIN));
						char head_buffer[sizeof(PACK_HEAD)] = { 0 };
						memcpy(head_buffer, &head, sizeof(PACK_HEAD));

						char buffer[sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN) + sizeof(int)] = { 0 };
						//拷贝包头
						memcpy(buffer, &head, sizeof(PACK_HEAD));
						//拷贝包体
						memcpy(buffer + sizeof(PACK_HEAD), &regist_login, sizeof(PACK_REGIST_LOGIN));
						//拷贝fd
						memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN), &epoll_events[i].data.fd, sizeof(int));
						if (res > 0)
						{
							cout << "msg=" << head.bodyType << endl;
							shm_fifo1.shm_write(buffer);
							this->epoll_recv(epoll_events[i].data.fd,buffer);
						}
					}
				}

				else
				{
					//客户端断开连接
					cout << "client error close" << endl;
					close(epoll_events[i].data.fd);
					break;
				}
				*/
			}
			
		}
	}
}
void EPollServer::Stop()
{

}
void EPollServer::epoll_recv(int fd, int msgid)
{
	MyTask* task;
	task = new MyTask(fd, msgid);
	pool->addTask(task);
	PrintLog::getInstance()->rec_packCounts++;
	PrintLog::getInstance()->print_log();
}

bool EPollServer::Epoll_Add(int fd, int event_type)
{
	//accept函数产生了新的套接字，要把这个新的套接字存入内核容器中
	this->event.data.fd = fd; //记录文件描述符
	this->event.events = event_type | EPOLLET;  //对这个套接字事件读事件
	//epoll_ctl函数的作用是管理红黑树实例上的节点，可以进行添加、删除、修改操作
	int res = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);//把服务器这个套接字存入epoll内部
	if (res < 0)
	{
		ERR_EXIT("epoll_ctl");
		return false;
	}
	return true;
}
bool EPollServer::Epoll_Del(int fd, int event_type)
{
	this->event.data.fd = fd;
	this->event.events = event_type; //对这个套接字事件感兴趣
	int res = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &event);  //把服务器这个套接字从epoll内部删除
	if (res < 0)
	{
		ERR_EXIT("epoll_ctl");
		return false;
	}
	return true;
}