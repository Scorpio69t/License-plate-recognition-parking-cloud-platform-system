#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <vector>
#include <iostream>
using namespace std;
//任务类是纯虚类。要继承这个类，在run方法里使用
class TaskBase
{
public:
	virtual void Run() = 0;
private:
	void* data;
	int fd;
};

class ThreadPool
{
public:
	ThreadPool(int maxCount = 20);
	~ThreadPool();
	void addTask(TaskBase* pTask);
	static void* Routinue(void* pData);//线程池线程主体函数
	//静态的原因pthread_ create是C语言函数，它的第三个参数是线程函数名
	//不能是类的方法，不会自带this指针
private:
	list <TaskBase*> taskList;
	vector<pthread_t> pthread_vector;
	pthread_mutex_t mutex;
	pthread_mutex_t listmutex;
	pthread_cond_t cond;
	int maxCount;
	bool isQuit;

};

#endif // !THREADPOOL_H


