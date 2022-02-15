#include "thread.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <Tool/singledb.h>
#include <Widget/main_in.h>
#include <Widget/main_out.h>
Thread *Thread::myThread=nullptr;
Thread::Thread()
{
    //定时器：每两分钟发一次心跳包
    heartbeat = new QTimer();
    connect(heartbeat, SIGNAL(timeout()), this, SLOT(send_heartbeat()));
    heartbeat->start(120000);
}
//心跳槽函数
void Thread::send_heartbeat()
{
    /*发到服务器*/
    PACK_HEAD head;
    char buffer[sizeof(PACK_HEAD)] = { 0 };
    memset(&head, 0, sizeof(PACK_HEAD));
    //类型是心跳包
    head.bodyType=HEART_TYPE;

    //拷贝包头，发送心跳包
    memcpy(buffer, &head, sizeof(PACK_HEAD));
    write(SingleDB::getInstance()->get_fd(), buffer, sizeof(PACK_HEAD));
    cout<<"HEART_TYPE"<<endl;
}
//外部公共接口 获取单例对象
Thread *Thread::getInstance()
{
    if(Thread::myThread==nullptr)
    {
        Thread::myThread=new Thread();
    }
    return Thread::myThread;
}

void Thread::run()
{
    //定义包头
    PACK_HEAD head;
    memset(&head, 0, sizeof(PACK_HEAD));
    while(true)
    {
        //读服务器发来的包头，没有就阻塞
        read(SingleDB::getInstance()->get_fd(), &head, sizeof(PACK_HEAD));
        //包体类型是登录包
        if (head.bodyType == LOGIN)
        {
            REGIST_LOGIN_RESULT result;
            //读服务器发来的包体
            read(SingleDB::getInstance()->get_fd(), &result, sizeof(REGIST_LOGIN_RESULT));
            //根据结果判断登录是否成功，发送成功或者失败信号
            if(result.result == -1)
            {
                emit Login(false);
            }
            else
            {
                //登录成功后会返回数据库没出车库的车数量
                car_amount = result.result;
                emit Login(true);
            }
            //清空
            memset(&result, 0x0, sizeof(REGIST_LOGIN_RESULT));
        }
        //包体类型是注册包
        else if (head.bodyType == REGISTER)
        {
            REGIST_LOGIN_RESULT result;
            //读服务器发来的包体
            read(SingleDB::getInstance()->get_fd(), &result, sizeof(REGIST_LOGIN_RESULT));
            //根据结果判断注册是否成功，发送成功或者失败信号
            if(result.result == -1)
            {
                emit Register(false);
            }
            else
            {
                emit Register(true);
            }
            memset(&result, 0x0, sizeof(REGIST_LOGIN_RESULT));
        }

        //包体类型是出场包
        else if (head.bodyType == CAR_GETOUT)
        {
            PACK_EXIT_BACK result;
            //读服务器发来的包体
            read(SingleDB::getInstance()->get_fd(), &result, sizeof(PACK_EXIT_BACK));
            //发送带出场包的信号
            emit out(result);
            memset(&result, 0x0, sizeof(PACK_EXIT_BACK));
        }
        //包体类型是车辆信息包
        else if (head.bodyType == CAR_MSG_TYPE)
        {
            PACK_ALLCARMSG_BACK result;
            //读服务器发来的包体
            read(SingleDB::getInstance()->get_fd(), &result, sizeof(PACK_ALLCARMSG_BACK));
            //发送带车辆信息包的信号
            emit car_msg(result);
            memset(&result, 0x0, sizeof(PACK_ALLCARMSG_BACK));
        }


        else if (head.bodyType == PHOTO_TYPE)
        {
            PACK_PHOTO_BACK result;
            //读服务器发来的包体
            cout<<"111"<<endl;
            read(SingleDB::getInstance()->get_fd(), &result, sizeof(PACK_PHOTO_BACK));
            cout<<"222"<<endl;
            emit photo_continue_in(result.result);
            memset(&result, 0x0, sizeof(PACK_ALLCARMSG_BACK));
        }
    }
}
