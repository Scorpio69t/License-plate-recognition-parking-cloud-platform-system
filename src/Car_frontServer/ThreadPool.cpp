#include "ThreadPool.h"
ThreadPool::ThreadPool(int maxCount)
{
	pthread_t pthread_id;
	this->maxCount = maxCount;
	pthread_mutex_init(&this->mutex, NULL);
	pthread_cond_init(&this->cond, NULL);
	this->isQuit = false;
	//线程池可以一开始就创建N个线程
	for (int i = 0; i < this->maxCount; i++)
	{
		pthread_create(&pthread_id, NULL, ThreadPool::Routinue, this);
		pthread_vector.push_back(pthread_id);
	}
}
ThreadPool::~ThreadPool()
{

}
void ThreadPool::addTask(TaskBase* pTask)
{
	pthread_mutex_lock(&mutex);

	this->taskList.push_back(pTask);
	pthread_cond_signal(&cond);

	pthread_mutex_unlock(&mutex);
}
void* ThreadPool::Routinue(void* pData)//线程池线程主体函数
{
	TaskBase* pTask;
	ThreadPool* pool = (ThreadPool*)pData;
	//cout << "create a new thread" << endl;
	while (!(pool->isQuit))
	{
		pthread_mutex_lock(&pool->mutex);

		//检查任务列表是否为空
		if (pool->taskList.empty())
		{
			//cout << "thread wait..." << endl;
			pthread_cond_wait(&pool->cond, &pool->mutex);

			pthread_mutex_unlock(&pool->mutex);
		}
		else//说明队列不为空 取任务
		{
			pTask = pool->taskList.front();//取出第一个任务
			pool->taskList.pop_front();//删除第一个任务

			pthread_mutex_unlock(&pool->mutex);
			//运行任务列表的任务
			pTask->Run();
		}
	}
}