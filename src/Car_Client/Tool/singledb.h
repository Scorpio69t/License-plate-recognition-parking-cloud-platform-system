#ifndef SINGLEDB_H
#define SINGLEDB_H
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include<arpa/inet.h>
#include <QString>
using namespace std;

#define ERR_EXIT(m) \
    do{ \
    perror(m); \
    exit(EXIT_FAILURE); \
    } while (0)

class SingleDB
{
public:
    //外部公共接口 获取model对象
    static SingleDB *getInstance();
    //socket执行
    int socket_connect();
    int get_fd();

    QString get_image_path();
    QString get_video_path();
    int get_size();

private:
    SingleDB();//构造私有化
    static SingleDB *myDB;//静态私成员变量
    int socketfd;
    char IP[64];
    int port;

    QString image_path;
    QString video_path;
    int size;
};

#endif // SINGLEDB_H
