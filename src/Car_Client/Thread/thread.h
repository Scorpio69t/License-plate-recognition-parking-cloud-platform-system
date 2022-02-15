#ifndef THREAD_H
#define THREAD_H
#include "Tool/package.h"
#include <QThread>
#include <iostream>
#include <QTimer>
using namespace std;
class Thread:public QThread
{
    Q_OBJECT
public:
    //外部公共接口 获取model对象
    static Thread *getInstance();
    int get_car_amount()
    {
        return car_amount;
    }
//    static FILE*fp;
//    static PACK_PHOTO photo;

signals:
    void Login(bool success);
    void Register(bool success);
    void out(PACK_EXIT_BACK );
    void car_msg(PACK_ALLCARMSG_BACK);
    void photo_continue_in(int result);

private:
    Thread();//构造私有化
    static Thread *myThread;//静态私成员变量
    int car_amount = 0;
    QTimer *heartbeat;


public slots:
    void send_heartbeat();
protected:
    void run();
};

#endif // THREAD_H
