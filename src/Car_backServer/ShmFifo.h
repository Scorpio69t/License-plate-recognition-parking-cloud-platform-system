#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include<iostream>
#include<malloc.h>
using namespace std;

typedef struct ShmHead
{
    int r_index;
    int w_index;
    int blocks;
    int block_size;
}SHM_HEAD_T;

class ShmFifo
{
public:
    SHM_HEAD_T* shm_start_addr;
    char* shm_data_add;//共享内存实际数据地址(真正存储数据开始的地址)
    int shm_id;//共享内存id
    ShmFifo(int key, int blocks, int block_size)
    {
        //如果内核中不存在键值与key相等的共享内存，则新建一个消息队列；如果存在这样的共享内存则报错
        this->shm_id = shmget(key, blocks * block_size, IPC_CREAT | IPC_EXCL);
        if (this->shm_id == -1) //已经创建过共享内存了
        {
            //如果内核中不存在键值与key相等的共享内存，则新建一个共享内存；如果存在这样的共享内存，返回此共享内存的标识符
            this->shm_id = shmget(key, blocks * block_size, IPC_CREAT);
            //共享内存起始地址，放头部结构体数据
            this->shm_start_addr = (SHM_HEAD_T*)shmat(this->shm_id, 0, 0);
            //存放数据实际地址
            this->shm_data_add = (char*)(this->shm_start_addr + 1);
        }
        else//第一次创建，要设置头地址和数据开始地址
        {
            //共享内存起始地址，放头部结构体数据
            this->shm_start_addr = (SHM_HEAD_T*)shmat(this->shm_id, 0, 0);
            //存放数据实际地址
            this->shm_data_add = (char*)(this->shm_start_addr + 1);
            this->shm_start_addr->blocks = blocks;
            this->shm_start_addr->block_size = block_size;
            this->shm_start_addr->w_index = 0;
            this->shm_start_addr->r_index = 0;
            

        }
    }
    char* shm_read()
    {
        char* buf = (char*)malloc(this->shm_start_addr->block_size);
        //把共享内存对应的位置数据拷贝到buf中
        memcpy(buf, this->shm_data_add + this->shm_start_addr->r_index * this->shm_start_addr->block_size, this->shm_start_addr->block_size);
        this->shm_start_addr->r_index = (this->shm_start_addr->r_index + 1) % this->shm_start_addr->blocks;
        return buf;
    }
    void shm_write(char* buf)
    {
        memcpy(this->shm_data_add + this->shm_start_addr->w_index * this->shm_start_addr->block_size, buf, this->shm_start_addr->block_size);
        //实现环形
        this->shm_start_addr->w_index = (this->shm_start_addr->w_index + 1) % this->shm_start_addr->blocks;
    }
};

