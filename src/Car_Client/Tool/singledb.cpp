#include "singledb.h"
#include <QFileDialog>
#include <QString>
SingleDB *SingleDB::myDB=nullptr;

SingleDB::SingleDB()
{
    QFile file("./data/set_data.txt");
    if(!file.open(QIODevice::ReadWrite))
    {
        return;
    }
    else
    {
        int line = 0;
        //判断文件是否已经读到末尾了
        while(!file.atEnd())
        {
            line++;
            QByteArray byte = file.readLine().trimmed();
            char* str = byte.data();
            //读取IP数据
            if(line == 1)
            {
                strcpy(this->IP,str);
            }
            //读取端口数据
            else if(line == 2)
            {
                QString num(str);
                this->port = num.toInt();
            }
            //读取IP数据
            else if(line == 3)
            {
                this->image_path =QString(str);
            }
            else if(line == 4)
            {
                this->video_path = QString(str);
            }
            //读取端口数据
            else if(line == 5)
            {
                QString num(str);
                this->size = num.toInt();
                file.close();
                return;
            }
        }
        file.close();
    }
}

//外部公共接口 获取单例对象
SingleDB *SingleDB::getInstance()
{
    if(SingleDB::myDB==nullptr)
    {
        SingleDB::myDB=new SingleDB();
    }
    return SingleDB::myDB;
}

//socket执行
int SingleDB::socket_connect()
{
    //创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == -1)
    {
        ERR_EXIT("socket");
        return -1;
    }
    //绑定IP和端口
    struct sockaddr_in ser_addr;
    //填充addr结构体
    ser_addr.sin_family = AF_INET;//协议家族 ipv4
    ser_addr.sin_port = htons(this->port);//网络字节序 主机字节序
    ser_addr.sin_addr.s_addr = inet_addr(this->IP);//手动获取服务器IP
    int ret = connect(sockfd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr_in));
    if (ret == -1)
    {
        ERR_EXIT("connect");
        return -1;
    }
    cout << "connect success" << endl;
    this->socketfd=sockfd;
    return 0;
}

int SingleDB::get_fd()
{
    return socketfd;
}

QString SingleDB::get_image_path()
{
    return image_path;
}

QString SingleDB::get_video_path()
{
    return video_path;
}

int SingleDB::get_size()
{
    return size;
}
